/**
 *
 * 共享内存区(内存映射文件)+posix信号量
 * 使用open打开
 */


#include <stdlib.h>
#include <stdio.h>
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
#include <fcntl.h>

#include <semaphore.h>
#include <sys/mman.h>


#define MAXLINE 1024

#define N_PLUS 1000000
#define SEM_NAME "/sem_mmap"//信号量由sem_open打开
#define MMAP_NAME "mmap.test"//内存映射文件，由open函数打开，路径名与信号量不一样

//处理信号函数
typedef void Sigfunc(int); //简化
Sigfunc *signal(int signo, Sigfunc *func);
void sig_child(int signo);
void sig_int(int signo);//

//2
//创建子进程的参数的结构
typedef struct {
		int *ptr;//共享内存地址
		int *nplus;//每个子进程统计服务次数的地址，也在共享内存区
		sem_t *sem_mutex;//使用基于文件的信号量同步
		sem_t *sem_nempty;
		sem_t *sem_nstored;
		int nloop;
	} ARG;
ARG arg;

void* producer(ARG *arg);
void* customer(ARG *arg);

int main(int argc, char** argv) {
	int fd, i, nloop, zero = 0;
	int *ptr;//指向共享存储区
	sem_t *sem_mutex;//使用信号量同步
	
	nloop = N_PLUS;
	//nloop = 1000;
	fd = open(MMAP_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	write(fd, &zero, sizeof(int));//往文件中写入数据
	//共享内存区大小跟文件大小一样，这里是一个int型的大小
	
	//把内存映射文件映射到调用进程的地址空间，unpv2 p248
	//第一个参数，被映射到进程地址空间的起始地址，NULL为系统分派
	//第二个参数是映射的字节数，，以页（pagesize）为单位，小于一个页的字节数按一页来处理
	//第三个参数，内存映射区的保护，这里能读写
	//第四个参数，MAP_SHARED表示调用进程对被映射的数据的修改对该共享内存的所有进程可见
	//第五个参数表示从被映射文件的第offset字节开始
	ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//close(fd);
	
	sem_mutex = sem_open(SEM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 1);
	sem_unlink(SEM_NAME);//删除信号量文件，程序退出就完全删掉
	
	setbuf(stdout, NULL);//设置标准输出非缓冲
	
	int nplus = 0;//每个进程计数次数
	
	///////////
	//1
	//父子进程对共享存储区的计数器各叠加N_PLUS次
	
	//child
	if(fork() == 0){
		for(i = 0; i < nloop; i++){
			sem_wait(sem_mutex);
			(*ptr)++;
			//printf("child: %d\n", (*ptr));
			sem_post(sem_mutex);
			
			//nplus++;
		}
		printf("child结束, 当前值*ptr: %d\n", *ptr);
		exit(0);	
	}
	
	//parent
	for(i = 0; i < nloop; i++){
			sem_wait(sem_mutex);
			(*ptr)++;
			//printf("parent: %d\n", (*ptr));
			sem_post(sem_mutex);
			
			//nplus++;
		}
	printf("parent结束, 当前值*ptr: %d\n", *ptr);
	
	munmap(ptr, sizeof(int));
	
	
	//////////////
	//2
	//一个生产者（父进程），10个（子进程）
	//消费者对共享内存区的计数器总共叠加N_PLUS次，
	//由生产者调度，最多有5个子进程同时获得消费者身份
	//同时把每个消费者叠加的次数写入共享存储区
	sleep(1);
	char buf[MAXLINE];
	printf("按enter继续 2）生产者-消费者...\n");
	read(0, buf, MAXLINE);
	
	memset(buf, 0, MAXLINE);
	write(fd, buf, 40);//往文件中写入数据
	void *vptr = mmap(NULL, sizeof(int) * 11, PROT_READ | PROT_WRITE,
											 MAP_SHARED, fd, 0);
	close(fd);
											 
	ptr = vptr;//计数器地址
	int nsize = 1;
/*	ARG arg;*/
	arg.nloop = N_PLUS;
	arg.nplus = ptr + nsize;//记录操作的次数起始地址，第一个子进程
	arg.ptr = ptr;
	
	arg.sem_mutex = sem_mutex;
	arg.sem_nempty = sem_open("/sem2", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 5);
	arg.sem_nstored = sem_open("/sem1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 0);
	sem_unlink("/sem1");
	sem_unlink("/sem2");
	
	//signal(SIGCHLD, sig_child);
	signal(SIGINT, sig_int);
	
	*ptr = 0;
	
	//消费者
	pid_t pids[10];
	for(int i = 0; i < 10; i++){
		if((pids[i] = fork()) == 0){
			//每个子进程存放操作的次数的地址，int型指针+1为+4字节,
			//mmap函数映射以页（pagesize）为单位，小于一个页的字节数按一页来处理
			arg.nplus += i * nsize;
			printf("arg.nplus: %p\n", arg.nplus);//打印每个子进程计数的地址
			customer(&arg);
		}
	}
	
	//生产者，
	producer(&arg);
	
	//所有操作完成后返回，杀死子进程
	for(int i = 0; i < 10; i++){
		kill(pids[i], SIGINT);
	}
   	
   	sleep(1);//等待子进程结束
   	printf("producer: %d, *ptr: %d\n", getpid(), *ptr);
   	
   	int sum = 0;
   	//取出每个子进程操作的次数
   	for(int i = 0; i < 10; i++){
		printf("parent: %d, *(arg.nplus): %d\n", getpid(), *(arg.nplus));
		sum += *(arg.nplus);
		arg.nplus += nsize;
	}
   	printf("parent: %d, total: %d\n", getpid(), sum);//
   	
   	unlink(MMAP_NAME);//删除映射文件
   	
   	//sleep(1);
    exit(0);
}


//signal
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
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        //printf("child %d terminated（子进程结束）\n", pid);
        ;
    }
    return;
}

void sig_int(int signo) {
    printf("child: %d, nplus: %d\n", getpid(), *(arg.nplus));
	exit(0);
}

void* producer(ARG *arg){
	while(*(arg->ptr) < arg->nloop){
		sem_wait(arg->sem_nempty);
		sem_post(arg->sem_nstored);
	}
	printf("producer: %d, *(arg->ptr): %d\n", getpid(), *(arg->ptr));
	return NULL;
}

void* customer(ARG *arg){
	while(1){
		sem_wait(arg->sem_nstored);
		sem_wait(arg->sem_mutex);
		if(*(arg->ptr) < arg->nloop){
			(*(arg->ptr))++;
			(*(arg->nplus))++;
		}
		sem_post(arg->sem_mutex);
		sem_post(arg->sem_nempty);
	}
}

