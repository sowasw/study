/**
 * 线程池回射服务器
 * 主线程accept加锁，条件变量
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h> 
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> /* inet(3) functions */
#include <errno.h> /* inet(3) functions */
#include <signal.h>
#include <sys/wait.h> 
#include <poll.h>
#include <limits.h>/*2.6.24以后没有open_max了*/
#include <pthread.h>
#include <signal.h> 
#include <sys/resource.h>/*void sig_int(int signo)*/

#define MAXLINE 1024
#define N_THREADS 5000


typedef struct {
    pthread_t thread_tid;
    long thread_count;
} Thread;
Thread* tptr;//线程数组首地址

#define MAXNCLI  N_THREADS
int clifd[MAXNCLI], iget, iput;


static int nthreads;
//socklen_t addrlen;
//pthread_mutex_t mlock;

pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;


/*
 * 
 */
void str_echo(int socket);//服务程序

//处理信号
typedef void Sigfunc(int); //简化
Sigfunc *signal(int signo, Sigfunc *func);
void sig_child(int signo);
void sig_int(int);

void thread_make(int);//线程创建

int main(int argc, char** argv) {
    int listenfd;
    int i, connfd;
    // pthread_t tid;
    socklen_t addrlen, clilen;
    
    struct sockaddr* cliaddr;
	cliaddr = (struct sockaddr*) malloc(sizeof (struct sockaddr));
	
    struct sockaddr_in seraddr;
    addrlen = sizeof (seraddr);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        fprintf(stderr, "监听套接字创建失败，退出！\n");
        exit(-1);
    }

    memset(&seraddr, 0, sizeof (seraddr));
    seraddr.sin_family = AF_INET;
    
    if(argc == 1){
    	if (inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr) <= 0) {
        	fprintf(stderr, "字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   		}
   		seraddr.sin_port = htons(9999);
    } else if(argc == 3) {
    	if (inet_pton(AF_INET, argv[1], &seraddr.sin_addr) <= 0) {
        	fprintf(stderr, "字节序转换失败！inet_pton error!\n");
        	exit(-1);
   		}
   		seraddr.sin_port = htons(atoi(argv[2]));
    } else {
    	fprintf(stderr, "不正确的命令行参数格式!\n");
        exit(-1);
    }

    if (bind(listenfd, (struct sockaddr*) &seraddr, sizeof (seraddr)) < 0) {
        fprintf(stderr, "绑定地址失败，退出！\n");
        exit(-1);
    }

    listen(listenfd, 10000);

    nthreads = N_THREADS;
    tptr = (Thread*) calloc(nthreads, sizeof (Thread));
    iget = iput = 0;

    for (i = 0; i < nthreads; i++) {
        thread_make(i);//创建线程池
    }
    signal(SIGINT, sig_int);
//    while (1)
//        pause();

	//主循环accept
    int nf = 0;
    for (;;) {
        printf("主线程accept循环次数:%d\n", ++nf);
        clilen = addrlen;
        connfd = accept(listenfd, cliaddr, &clilen);

		//互斥锁，条件变量，分派任务，唤醒线程
        pthread_mutex_lock(&clifd_mutex);
        clifd[iput] = connfd;
        if (++iput == MAXNCLI)//
            iput = 0;
        if (iput == iget)
            fprintf(stdout, "iput = iget = %d", iput);
        pthread_cond_signal(&clifd_cond);
        pthread_mutex_unlock(&clifd_mutex);

    }


    return 0;
}

void str_echo(int socket) {
    ssize_t n;
    char buf[MAXLINE + 1];
again:

    while ((n = read(socket, buf, MAXLINE)) > 0) {
        buf[n] = 0;
        //        printf("%s", buf);
        write(socket, buf, n);
    }
    if (n < 0 && errno == EINTR)
        goto again;
    if (n < 0) {
        printf("read error！\n");

        //close(socket);
        return;
    }
    //    printf("读取0字节！\n");
}

Sigfunc* signal(int signo, Sigfunc *func) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; //|=按位或之后赋值
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);
    return (oact.sa_handler);
}

void sig_child(int signo) {
    pid_t pid;
    int stat;

    //    pid = wait(&stat);
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)//正确应该调用waitpid,unp书上p110
        printf("child %d terminated（子进程结束）\n", pid);
    return;
}

void sig_int(int signo) {
    void pr_cpu_time(void);

    pr_cpu_time();
    exit(0);
}

//cpu时间统计
void pr_cpu_time(void) {/*for void sig_int(int signo)*/
    double user, sys;
    struct rusage myusage, childusage;

    if (getrusage(RUSAGE_SELF, &myusage) < 0) {
        fprintf(stdout, "getusage error\n");
        exit(1);
    }
    if (getrusage(RUSAGE_CHILDREN, &childusage) < 0) {
        fprintf(stdout, "getusage error\n");
        exit(1);
    }

    user = (double) myusage.ru_utime.tv_sec + myusage.ru_utime.tv_usec / 1000000.0;
    user += (double) childusage.ru_utime.tv_sec + childusage.ru_utime.tv_usec / 1000000.0;

    sys = (double) myusage.ru_utime.tv_sec + myusage.ru_utime.tv_usec / 1000000.0;
    sys += (double) childusage.ru_utime.tv_sec + childusage.ru_utime.tv_usec / 1000000.0;

    printf("\nuser time = %g, sys time = %g\n", user, sys);
}


//这里void*转int 在g++不能编译，8位指针转4位 有数据丢失
//可以用long型代替
void thread_make(int i) {
    void * thread_main(void *);

    pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
    return;
}

void * thread_main(void * arg) {//线程main函数
    int connfd;
  
    printf("thread: %d starting...\n", (int) arg);
    int nf = 0;
    for (;;) {
        printf("thread: %d tid: %lu开始第%d次等待\n", (int) arg, pthread_self(), ++nf);
        
        pthread_mutex_lock(&clifd_mutex);//条件变量+锁，等待业务
        while (iput == iget) {
            pthread_cond_wait(&clifd_cond, &clifd_mutex);
        }
        connfd = clifd[iget];
        if (++iget == MAXNCLI)
            iget = 0;
        pthread_mutex_unlock(&clifd_mutex);
        
        tptr[(int) arg].thread_count++;//服务次数
        printf("thread: %d tid: %lu第%d次服务\n", (int) arg, pthread_self(), nf);

        str_echo(connfd);//业务程序，回射
        close(connfd);
    }
}
