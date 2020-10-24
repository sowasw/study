#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/syscall.h>//gettid(): tid = syscall(SYS_gettid);

/*
*验证：
*gettid,errno 是线程独有的（gettid是内核的函数，用户程序用syscall(SYS_gettid)返回）
*getpid 为主线程进程id，gettid是各线程在内核的进程id
*pthread_self()返回的线程id在线程销毁后，再创建新线程会重新启用（陈硕）
*/

void print_tid(const char* thread_name);
void* thread_fn(void* arg);
void* thread_fn2(void* arg);

typedef struct {
		int *np;
		int nplus;
		} PLUSARG;
void* thread_plus(void* arg);
void* thread_plus2(void* arg);

int main(int argc, char** argv){
	int i = 0;
	int err;
	int nthreads = 10;
	pthread_t newtid[nthreads];
	void* tret;
	int num = 0;

	for(i = 0; i < nthreads; i++){
		newtid[i] = -1;
    } 
    
	{
	err = pthread_create(&newtid[0], NULL, thread_fn, NULL);
	if(err != 0)exit(1);
	pthread_create(&newtid[1], NULL, thread_fn2, NULL);
	}

	print_tid("main thread: ");
	pthread_join(newtid[0], &tret);
	pthread_join(newtid[1], &tret);

    //pthread_self()返回的线程id在线程销毁后，再创建新线程会重新启用（陈硕）
    pthread_create(&newtid[4], NULL, thread_fn2, NULL);
    pthread_join(newtid[4], &tret);


	exit(0);
}

void print_tid(const char* thread_name){
	pthread_t tid;
	pid_t pid;
	long int gettid_id;


	pid = getpid();
	tid = pthread_self();
	gettid_id = syscall(SYS_gettid);

    int n = 5;
    while(n-->0){
	   printf("%s pid: %lu, tid: %lu, (0x%lx), gettid_id: %ld, errno: %d\n", thread_name, 
		(unsigned long)pid, (unsigned long)tid, (unsigned long)tid, gettid_id, errno);
		//perror("error: ");
		sleep(1);
	}
}

void print_tid2(const char* thread_name){
	pthread_t tid;
	pid_t pid;
	long int gettid_id;


	pid = getpid();
	tid = pthread_self();
	//gettid_id = gettid();
	gettid_id = syscall(SYS_gettid);

    int n = 5;
    while(n-->0){
        errno ++;
	    printf("%s pid: %lu, tid: %lu, (0x%lx), gettid_id: %ld, errno: %d\n", thread_name, 
		(unsigned long)pid, (unsigned long)tid, (unsigned long)tid, gettid_id, errno);
		//perror("error: ");
		sleep(1);
	}
}

void* thread_fn(void* arg){
	print_tid("new thread: ");
	return ((void*)0);
}

void* thread_fn2(void* arg){
	print_tid2("new thread: ");
	return ((void*)0);
}


