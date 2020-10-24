/*
 * epoll回射服务器,ET模式+非阻塞I/O
 * 命令行第一个参数指定服务器IP，第二个参数指定端口号，
 * 如果没有命令行参数，则默认指定INADDR_ANY，端口号9999,其他并发服务器就是如此指定的
 */

//#include "wxl_head.h"
#include "wxl_tcp.h"

#define MAXNFD 2048

int main(int argc, char **argv) {
	int listenfd, connfd, epfd, sockfd;
	int nready, i, nread, nwrite;
	char buf[MAXLINE];
	int flags;

	mysignal(SIGPIPE, sig_pipe);//忽略SIGPIPE信号
	
	socklen_t clilen;
	struct sockaddr_in cliaddr;
	
	listenfd = wxl_tcp_listen(argc, argv);//建立监听套接字//wxl_tcp.h
	
	epfd = epoll_create(1);
	
	struct epoll_event revents[MAXNFD];//epoll检测到的事件返回到这个事件数组
	
	//把监听套接字注册到epfd
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	epoll_ctl (epfd, EPOLL_CTL_ADD, listenfd, &ev);
	
/*	//把标准输入注册到epfd*/
/*	ev.events = EPOLLIN;*/
/*	ev.data.fd = 0;*/
/*	//epoll_ctl (epfd, EPOLL_CTL_ADD, 0, &ev);*/

	int n_ep_ret = 0;
	while(true){
		if((nready = epoll_wait(epfd, &revents[0],MAXNFD, 0)) > 0) {
			for(i = 0; i < nready; i++) {
				sockfd = revents[i].data.fd;
				
				printf("epoll_wait return %d\n", ++n_ep_ret);//测试LT，ET
				
				if(sockfd == listenfd) {		//接受连接
					if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
						if(errno == EINTR)
							continue;
						else{
							printf("accept error\n");
							exit(-1);
						}
					}
					
					//注册连接的客户事件
					//ev.events = EPOLLIN;				//LT
					ev.events = EPOLLIN | EPOLLET;	//ET
					
					ev.data.fd = connfd;
					epoll_ctl (epfd, EPOLL_CTL_ADD, connfd, &ev);
					
					//设置非阻塞
					flags = fcntl(connfd, F_GETFL, 0);
					fcntl(connfd, F_SETFL, flags | O_NONBLOCK);	

				} else {						//处理客户服务
					if(revents[i].events & EPOLLIN) {
					
/*						//一次只读一个字节，用于测试LT，ET模式是否持续触发*/
/*						//ET模式，如果没有读完，新数据到来之前不会再触发；新数据到来会触发*/
/*						nread = read(sockfd, buf, 1);//一次只读一个字节，用于测试LT，ET*/
/*						nwrite = write(sockfd, buf, nread);*/
						
						/*ET */
						//read
						//非阻塞模式一定要循环读完数据
						while(true) { 
							nread = read(sockfd, buf, MAXLINE);
							//nread = read(sockfd, buf, 1);//一次只读一个字节
							
							if(nread > 0) {				//读到数据，回射
								nwrite = write(sockfd, buf, nread);
								if(nwrite < 0) {
									if (errno == EPIPE) {	//往已收到RST的套接字发送数据导致本错误，并且收到内核的SIGPIPE信号
										printf("write error, errno = EPIPE\n");
										epoll_ctl (epfd, EPOLL_CTL_DEL, connfd, &ev);
										close(sockfd);
									}
								}

							} else if(nread < 0) {	
								if (errno == EWOULDBLOCK) {	//读完数据
									//printf("errno == EWOULDBLOCK\n");
									break;
									
								} else {					//read错误
									printf("read error on socket\n");
									exit(-1);
								}

							} else if(nread == 0) {			//处理客户关闭连接
								epoll_ctl (epfd, EPOLL_CTL_DEL, connfd, &ev);
								close(sockfd);
								break;
							}
						}// while 
						/*ET */
						
					}
				}//else//处理客户服务
			}//for
		}//nready > 0
	}//while
	
}//main
