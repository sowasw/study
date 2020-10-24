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

#define MAXLINE 1024

#ifndef OPEN_MAX
#define OPEN_MAX 1024//临时最大描述符个数默认值1024
#endif

#ifndef INFTIM     /*poll timeout参数，单位为毫秒， -1表示阻塞*/
#define INFTIM -1  
#endif             

#ifndef _POLL_IN
//#define _POLL_IN POLLIN
#define _POLL_IN POLLRDNORM
#endif

#define SERV_PORT 9999

void str_echo(int socket); //回射程序，处理完套接字才返回，这里没有用到

int main(int argc, char** argv) {
    int i, maxi, maxfd;
    int listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, seraddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        fprintf(stderr, "监听套接字创建失败，退出！\n");
        exit(-1);
    }

    memset(&seraddr, 0, sizeof (seraddr));
    seraddr.sin_family = AF_INET;
    if (argc == 1) {
        seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        seraddr.sin_port = htons(SERV_PORT);
    } else if (argc == 3) {
        if (inet_pton(AF_INET, argv[1], &seraddr.sin_addr) <= 0){
        	fprintf(stderr, "字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }
        
        seraddr.sin_port = htons(atoi(argv[2]));
    } else {
        fprintf(stderr, "请输入正确的参数形式： cmd host port\n");
        exit(-1);
    }

    if (bind(listenfd, (struct sockaddr*) &seraddr, sizeof (seraddr)) < 0) {
        fprintf(stderr, "绑定地址失败，退出！\n");
        exit(-1);
    }

    listen(listenfd, 1024);
    
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1;
    maxi = 0;

    int ind = 0;
    while (1) {
        //fprintf(stderr, "开始循环第%d次\n", ++ind);
        if ((nready = poll(client, maxi + 1, 10)) < 0) {
            fprintf(stderr, "poll返回错误\n");
            exit(1);
        }
        if (nready == 0) 
            continue;
            
        //fprintf(stderr, "poll返回就绪socket数: %d\n", nready);
        if (client[0].revents & POLLRDNORM) {
            //fprintf(stderr, "开始新联接!\n");
            clilen = sizeof (cliaddr);
            connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);
            //fprintf(stderr, "accept返回!\n");
            if (connfd > -1) {
                char buffer[20];
                fprintf(stderr, "连接成功！客户端地址：%s,端口号：%d\n",
                        inet_ntop(AF_INET, &cliaddr.sin_addr, buffer, 20),
                        ntohs(cliaddr.sin_port));
            }
            if (connfd < 0) {//unp,p108处理被中断的accept
                if (errno == EINTR)
                    continue;
                else {
                    fprintf(stderr, "accept返回错误\n");
                    exit(1);
                }
            }

            for (i = 0; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    //fprintf(stderr, "添加连接套接字：%d\n", connfd);
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == OPEN_MAX) {
                fprintf(stderr, "too many clients");
                exit(-1);
            }
            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;

        }

        for (i = 1; i <= maxi; i++) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        fprintf(stderr, "客户端reset!\n");
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        fprintf(stderr, "read error!\n");
                        exit(-1); //read error
                    }

                } else if (n == 0) {
                    fprintf(stderr, "客户端关闭已关闭，关闭连接\n");
                    close(sockfd);
                    client[i].fd = -1;
                } else {
                    if (write(sockfd, buf, n) < 0) {
                        fprintf(stderr, "write error\n");
                        exit(1);
                    }
                }

                if (--nready <= 0)
                    break;
            }
        }

        //fprintf(stderr, "一次循环结束\n");
    }
    
    return 0;
}

void str_echo(int socket) {
    ssize_t n;
    char buf[MAXLINE + 1];
    memset(buf, 0, MAXLINE + 1);
again:

    while ((n = read(socket, buf, MAXLINE)) > 0) {
        if (write(socket, buf, n) < 0) {
            fprintf(stderr, "write error\n");
            return;
        }
    }
    if (n < 0 && errno == EINTR)
        goto again;
    if (n < 0) {
        fprintf(stderr, "读取错误！\n");

        return;
    }
}

