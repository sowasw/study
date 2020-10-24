#include <sys/eventfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h>

#define MAX_EP_REVENTS 10
#define MAXLINE 1024

struct Thread {
	pthread_t tid;
	int evtfd;
	int epfd;
	uint64_t count;
	char buf[MAXLINE];
};

void *dowrite(void *arg);
void *doread(void *arg);

int main() {
	struct Thread wthread, rthread;
	
	memset(&wthread, 0, sizeof(struct Thread));
	memset(&rthread, 0, sizeof(struct Thread));
	
	wthread.evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	//rthread.evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	rthread.evtfd =wthread.evtfd;
	
	wthread.epfd = epoll_create(10);
	rthread.epfd = epoll_create(10);
	
	pthread_create(&wthread.tid, NULL, &dowrite, (void*)&wthread);
	pthread_create(&rthread.tid, NULL, &doread, (void*)&rthread);
	
/*	sleep(3);*/
/*	wthread.evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);*/
/*	rthread.evtfd =wthread.evtfd;*/
	
	pthread_join(wthread.tid, NULL);
	pthread_join(rthread.tid, NULL);
	
	return 0;
}

void *dowrite(void *arg) {
	struct Thread *tptr = (struct Thread*)arg;

/*	struct epoll_event revents[MAX_EP_REVENTS];//epoll检测到的事件返回到这个事件数组*/
/*	*/
/*	//注册到epfd*/
/*	struct epoll_event ev;*/
/*	ev.events = EPOLLIN;*/
/*	ev.data.fd = tptr->evtfd;*/
/*	epoll_ctl (tptr->epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);*/
	
	int nwrite = 0;
	tptr->count = 0;
	while(1) {
		if((nwrite = write(tptr->evtfd, &tptr->count, sizeof(uint64_t))) > 0){
			tptr->count++;
			sleep(1);
			//printf("??\n");
		}
			
	}
}

void *doread(void *arg) {
	struct Thread *tptr = (struct Thread*)arg;

	struct epoll_event revents[MAX_EP_REVENTS];//epoll检测到的事件返回到这个事件数组
	
	//tptr->epfd = epoll_create(10);
	//注册到epfd
	//sleep(5);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = tptr->evtfd;
	epoll_ctl (tptr->epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
	
	int nready = 0;
	int sockfd;
	int nread = 0;
	while(1){
		//printf("------------\n");
		//sleep(2);
		if((nready = epoll_wait(tptr->epfd, &revents[0],1024, -1)) > 0) {
			for(int i = 0; i < nready; i++) {
				//printf("++++\n");
				sockfd = revents[i].data.fd;
				if(sockfd == tptr->evtfd) {				
					nread = read(tptr->evtfd, &tptr->count, sizeof(uint64_t));
					
					printf("%ld\n", tptr->count);
				
				} 
			}
		}
	}
}
