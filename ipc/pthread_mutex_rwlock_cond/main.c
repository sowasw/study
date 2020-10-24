/**
 *
 * 线程互斥锁，读写锁，条件变量
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


#define MAXLINE 1024

#define N_PLUS 1000000

pthread_mutex_t nlock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t alock = PTHREAD_RWLOCK_INITIALIZER;


void* doit(void* arg);//互斥锁

//互斥锁+读写锁
void* doit1(void* arg);//互斥锁
void* test_rwlock1(void* arg);//读写锁
void* test_rwlock2(void* arg);//互斥锁+读写锁
void* test_rwlock3(void* arg);//互斥锁+读写锁

//long n = 0;
int array[4] = { 0 };

//生产者-消费者
//多个消费者线程对一个变量叠加N_PLUS次
int iput = 0, iget = 0;
pthread_cond_t procond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t prolock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t csrlock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t ncsrcond = PTHREAD_COND_INITIALIZER;
void* producer(void* arg);
void* customer(void* arg);


int main(int argc, char** argv) {
	pthread_t tid;
	long n = 0;
	
	//互斥锁，分别对n叠加N_PLUS次
	pthread_create(&tid, NULL, doit, &n);
	doit(&n);
	pthread_join(tid, NULL);
	printf("n: %ld\n", n);

	//互斥锁+读写锁，对同一个变量n叠加N_PLUS次
	n = 0;
	pthread_t tids[4];

	pthread_create(&tids[0], NULL, test_rwlock1, &n);//读锁，在n没有达到N_PLUS获得，获得锁次数存入array 0
	pthread_create(&tids[1], NULL, test_rwlock2, &n);//写锁+互斥锁叠加n，获得锁次数存入array 1
	pthread_create(&tids[2], NULL, test_rwlock3, &n);//写锁+互斥锁叠加n，获得锁次数存入array 2
	//doit1(&n);//互斥锁对n叠加，获得锁的次数存入array[3]
	pthread_create(&tids[4], NULL, doit1, &n);//写锁+互斥锁叠加n，获得锁次数存入array 2

	pthread_join(tids[0], NULL);
	pthread_join(tids[1], NULL);
	pthread_join(tids[2], NULL);

	printf("n: %ld\n", n);
	for (int i = 0; i < 4; ++i)
	{
		printf("array[%d]: %d\n", i, array[i]);
	}

	printf("total（总共对n叠加次数）: %d\n", array[1] + array[2] + array[3]);

//生产者-消费者
//多个消费者线程对一个变量叠加N_PLUS次,由生产者调度
	n = 0;
	pthread_t tids1[10];
	for (int i = 0; i < 10; ++i)
	{
		pthread_create(&tids1[i], NULL, customer, &n);
	}

	producer(&n);
	for (int i = 0; i < 10; ++i)
	{
		pthread_join(tids1[i], NULL);
	}

    
    return 0;
}

void* doit(void* arg){
	for(int i = 0; i < N_PLUS; i++){
		pthread_mutex_lock(&nlock);
		(*(long*)arg)++;
		//*(long*)arg++;//error
		
		//n++;
		pthread_mutex_unlock(&nlock);
	}
}

void* doit1(void* arg){
	while(1) {
		pthread_mutex_lock(&nlock);
		if((*(long*)arg) < N_PLUS){
				(*(long*)arg)++;
				array[3]++;
			}else{
				printf("doit1, n: %ld 获得互斥锁次数array[3]: %d\n", (*(long*)arg), array[3]);
				pthread_mutex_unlock(&nlock);
				return NULL;
			}
		pthread_mutex_unlock(&nlock);
	}
}

void* test_rwlock1(void* arg){
	while(1){
		pthread_rwlock_rdlock(&alock);
		if((*(long*)arg) < N_PLUS)
		//if((array[1] + array[2]) < N_PLUS)
			array[0]++;
		else{
			pthread_rwlock_unlock(&alock);
			printf("获得读锁次数array[0]: %d\n", array[0]);
			return NULL;
		}
		pthread_rwlock_unlock(&alock);
	}
}

void* test_rwlock2(void* arg){
	while(1){
		pthread_rwlock_wrlock(&alock);
		pthread_mutex_lock(&nlock);
		if((*(long*)arg) < N_PLUS){
			array[1]++;
			(*(long*)arg)++;
		}else{
			pthread_mutex_unlock(&nlock);
			pthread_rwlock_unlock(&alock);
			printf("获得写锁互斥锁次数array[1]: %d\n", array[1]);
			return NULL;
		}
		pthread_mutex_unlock(&nlock);
		pthread_rwlock_unlock(&alock);
	}
}

void* test_rwlock3(void* arg){
	while(1){
		pthread_rwlock_wrlock(&alock);
		pthread_mutex_lock(&nlock);
		if((*(long*)arg) < N_PLUS){
			array[2]++;
			(*(long*)arg)++;
		}else{
			pthread_mutex_unlock(&nlock);
			pthread_rwlock_unlock(&alock);
			printf("获得写锁互斥锁次数array[2]: %d\n", array[2]);
			return NULL;
		}
		pthread_mutex_unlock(&nlock);
		pthread_rwlock_unlock(&alock);
	}
}

void* producer(void* arg){
	while(1){
		if((*(long*)arg) < N_PLUS){
			pthread_mutex_lock(&prolock);//这里使用轮询，
			if (++iput == 5){
				iput = 0;
			}
			pthread_cond_signal(&procond);
			
			pthread_mutex_unlock(&prolock);
		}else{
			printf("n: %ld\n", *(long*)arg);
			exit(0);
		}
	}
}

void* customer(void* arg){
	while(1){
		pthread_mutex_lock(&prolock);
		while(iget == iput)
			pthread_cond_wait(&procond, &prolock);
		if(++iget == 5){
			iget = 0;
		}
		pthread_mutex_unlock(&prolock);	

		pthread_mutex_lock(&nlock);
		if((*(long*)arg) < N_PLUS)
			(*(long*)arg) += 10;
		else{
			pthread_mutex_unlock(&nlock);
			return NULL;
		}
		pthread_mutex_unlock(&nlock);
	}
}
