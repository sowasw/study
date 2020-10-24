/*
 * epoll回射服务器,LT模式
 * 命令行第一个参数指定服务器IP，第二个参数指定端口号，
 * 如果没有命令行参数，则默认指定INADDR_ANY，端口号9999,其他并发服务器就是如此指定的
 */

#include "wxl_tcp.h"

int main(int argc, char **argv) {
	int listenfd, connfd, epfd, sockfd;
	int nready, i, nread;
	char buf[MAXLINE];
	
	socklen_t clilen;
	struct sockaddr_in cliaddr;
	
	listenfd = wxl_tcp_listen(argc, argv);//建立监听套接字//wxl_tcp.h
	
	epfd = epoll_create(1);
	
	struct epoll_event revents[1024];//epoll检测到的事件返回到这个事件数组
	
	//把监听套接字注册到epfd
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	epoll_ctl (epfd, EPOLL_CTL_ADD, listenfd, &ev);
	
	//把标准输入注册到epfd
	ev.events = EPOLLIN;
	ev.data.fd = 0;
	epoll_ctl (epfd, EPOLL_CTL_ADD, 0, &ev);
	
	while(true){
		if((nready = epoll_wait(epfd, &revents[0],1024, -1)) > 0) {
			for(i = 0; i < nready; i++) {
				sockfd = revents[i].data.fd;
				if(sockfd == 0) {					//处理标准输入
					nread = read(0, buf, MAXLINE);
					write(1, buf, nread);//写到标准输出
				
				} else if(sockfd == listenfd) {		//接受连接
					//accept
					if((connfd = accept(listenfd, 
								(struct sockaddr*)&cliaddr, &clilen)) < 0) {
						if(errno == EINTR)
							continue;
						else{
							printf("accept error\n");
							exit(-1);
						}
					}
				
					//注册连接的客户事件
					ev.events = EPOLLIN | EPOLLOUT;
					ev.data.fd = connfd;
					epoll_ctl (epfd, EPOLL_CTL_ADD, connfd, &ev);
							
				} else {							//处理客户服务
					//读
					if(revents[i].events & EPOLLIN) {
						nread = read(revents[i].data.fd, buf, MAXLINE);
					
					//写，回射
						if(nread > 0 && revents[i].events & EPOLLOUT){
								//write(revents[i].data.fd, buf, nread);
								int nwrite = write(sockfd, buf, nread);
								if(nwrite < 0) {
									if (errno == EPIPE) {	//往已收到RST的套接字发送数据导致本错误，并且收到内核的SIGPIPE信号
										printf("write error, errno = EPIPE\n");
										epoll_ctl (epfd, EPOLL_CTL_DEL, connfd, &ev);
										close(sockfd);
									}
								}
						}
					//连接中断后删除事件	
						if(nread == 0){
							epoll_ctl (epfd, EPOLL_CTL_DEL, connfd, &ev);
							close(revents[i].data.fd);
						}
					}
					
				}//else//处理客户服务
				
			}//for
		
		}//nready > 0
		
	}//while
	
}//main
