/**
 *
 * posix信号量
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


#define MAXLINE 1024

#define N_PLUS 1000000
#define SEM_NAME "/testsem1"

sem_t *testsem_mutex;//互斥信号量
long n = 0;

//1线程之间同步
void* doit(void* arg);//使用互斥信号量

//2生产者调度消费者
sem_t *testsem_nempty;
sem_t *testsem_nstored;
void* producer(void* arg);
void* customer(void* arg);

//3进程之间同步
sem_t *testsem_nempty1;
sem_t *testsem_nstored1;
#define N_PLUS1 1000000
void* producer1(void* arg);
void* customer1(void* arg);
void print_n(long n);

//处理信号函数
typedef void Sigfunc(int); //简化
Sigfunc *signal(int signo, Sigfunc *func);
void sig_child(int signo);
void sig_int(int signo);//退出并打印n的值

//4基于内存的信号量
sem_t *memsem_mutex;
void* doit1(void* arg);


int main(int argc, char** argv) {
	pthread_t tid;
	//long n = 0;
	
	sem_t *testsem;
	testsem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	//创建的有名信号量在/dev/shm目录下，名字色sem.SEM_NAME
	
	testsem_mutex = sem_open("/testsem_mutex", O_CREAT | O_EXCL, 
					S_IRUSR | S_IWUSR, 1);	//出错表示已存在，返回SEM_FAILED
	if(testsem_mutex == SEM_FAILED)
		testsem_mutex = sem_open("/testsem_mutex", O_RDWR);
											//打开已存在的信号量，不用第三四个参数
	
	//1
	//互斥信号量，分别对n叠加N_PLUS次
	pthread_create(&tid, NULL, doit, &n);
	doit(&n);
	pthread_join(tid, NULL);
	printf("testsem_mutex, n: %ld\n", n);

//2
//生产者-消费者
//最多5个消费者线程对一个变量总共叠加N_PLUS次,由生产者调度
	n = 0;
	pthread_t tids1[10];
	testsem_nempty = sem_open("/testsem_nempty", O_RDWR | O_CREAT, 
												S_IRUSR | S_IWUSR, 5);
	testsem_nstored = sem_open("/testsem_nstored", O_RDWR | O_CREAT, 
												S_IRUSR | S_IWUSR, 0);
	for (int i = 0; i < 10; ++i)
	{
		pthread_create(&tids1[i], NULL, customer, &n);
	}

	//pthread_create(&tid, NULL, producer, &n);
	producer(&n);
/*	for (int i = 0; i < 10; ++i)*/
/*	{*/
/*		pthread_join(tids1[i], NULL);*/
/*	}	*/
	
	char buf[MAXLINE];
	printf("按enter继续...\n");
	read(0, buf, MAXLINE);
	//3
	//需要注释掉pthread_join()//2
	//进程间用信号量同步
	//两个子进程总共对n叠加N_PLUS1次，由父进程调度
	//两个子进程中n最终值相加等于N_PLUS1，在信号处理函数中打印n的值
	signal(SIGCHLD, sig_child);
	signal(SIGINT, sig_int);
	
	testsem_nempty1 = sem_open("/testsem_nempty1", O_RDWR | O_CREAT, 
												S_IRUSR | S_IWUSR, 2);
	testsem_nstored1 = sem_open("/testsem_nstored1", O_RDWR | O_CREAT, 
												S_IRUSR | S_IWUSR, 0);
	n = 0;
	pid_t pids[2];
	for(int i = 0; i < 2; i++){
		if((pids[i] = fork()) == 0){
			customer1(&n);
		}
	}
	producer1(&n);
	
   	//pause();
   	//sleep(1);
   	for(int i = 0; i < 2; i++){
		kill(pids[i], SIGINT);
	}
	
	//4
	//使用内存信号量，两个线程分别对n1叠加N_PLUS次
/*	sleep(1);*/
	long n1 = 0;
	memsem_mutex = malloc(sizeof(sem_t));
	//内存信号量用sem_init初始化，第二个参数为0表示此信号量是线程共享，
	//大于0则为进程共享，且必须在共享内存区，
	//第三个参数为其值，与sem_open的value一样
	if(sem_init(memsem_mutex, 0, 1) < 0)
		printf("sem_init error\n");
	pthread_create(&tid, NULL, doit1, &n1);
	doit1(&n1);
	pthread_join(tid, NULL);
	printf("use memsem_mutex, n1: %ld\n", n1);
	sem_destroy(memsem_mutex);
   	
    exit(0);
}

//1
void* doit(void* arg){
	for(int i = 0; i < N_PLUS; i++){
		//wait
		sem_wait(testsem_mutex);
		(*(long*)arg)++;
		sem_post(testsem_mutex);
		//post
	}
}

//2
void* producer(void* arg){
	while((*(long*)arg) < N_PLUS){
		sem_wait(testsem_nempty);
/*		sem_wait(testsem_mutex);*/
/*		sem_post(testsem_mutex);*/
		sem_post(testsem_nstored);
	}
	printf("producer, n: %ld\n", *(long*)arg);
	return NULL;
}

void* customer(void* arg){
	while(1){
		sem_wait(testsem_nstored);
		sem_wait(testsem_mutex);
		if((*(long*)arg) < N_PLUS){
			(*(long*)arg) += 10;
		}
		sem_post(testsem_mutex);
		sem_post(testsem_nempty);
	
	}
}

//3
void* producer1(void* arg){
	while((*(long*)arg) < N_PLUS1){
		sem_wait(testsem_nempty1);
		(*(long*)arg) += 1;
		sem_post(testsem_nstored1);
	}
	printf("producer1, n: %ld\n", *(long*)arg);
	return NULL;
}

void* customer1(void* arg){
	while(1){
		sem_wait(testsem_nstored1);
		//sem_wait(testsem_mutex);
		if((*(long*)arg) < N_PLUS1){
			(*(long*)arg) += 1;
			//printf("customer1, pid: %d, n: %ld\n", getpid(), *(long*)arg);
		}
		//sem_post(testsem_mutex);
		sem_post(testsem_nempty1);
	}
}

void print_n(long n){
	printf("child: %d, n: %ld\n", getpid(), n);
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
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)//正确应该调用waitpid,unp书上p110
        printf("child %d terminated（子进程结束）\n", pid);
    return;
}

void sig_int(int signo) {
    print_n(n);
    
    exit(0);
}

//4
void* doit1(void* arg){
	for(int i = 0; i < N_PLUS; i++){
		//wait
		sem_wait(memsem_mutex);
		(*(long*)arg)++;
		sem_post(memsem_mutex);
		//post
	}
}
