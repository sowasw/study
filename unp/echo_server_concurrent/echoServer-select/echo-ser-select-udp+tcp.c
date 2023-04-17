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

//#define MAX_CLIENT FD_SETSIZE
#define MAX_CLIENT 1010

#define SERV_PORT 9999
#define SERV_PORT_UDP 9998

void str_echo(int socket);//回射程序，处理完套接字才返回，这里没有用到

int main(int argc, char** argv) {
    
    int i, maxi, maxfd;
    int listenfd, connfd, sockfd;
    int nready, client[MAX_CLIENT]; 
    //fprintf(stderr, "MAX_CLIENT:%d\n", MAX_CLIENT);
    ssize_t n;
    fd_set rset, allset;
    char buf[1024];
    char bufudp[1024];
    socklen_t clilen;
    struct sockaddr_in cliaddr, seraddr;

    // udp
    int udpsockfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in udpsaddr,udpcaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0 || udpsockfd < 0) {
        fprintf(stderr, "监听套接字或udp套接字创建失败，退出！\n");
        exit(-1);
    }

    memset(&seraddr, 0, sizeof (seraddr));
    memset(&udpsaddr,0,sizeof(udpsaddr));
    seraddr.sin_family = AF_INET;
    udpsaddr.sin_family = AF_INET;
 	//udpsaddr.sin_port = htons(6000);
	//udpsaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (argc == 1) {
        seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        seraddr.sin_port = htons(SERV_PORT);

        udpsaddr.sin_port = htons(SERV_PORT_UDP);
	    //udpsaddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
        udpsaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else if (argc == 4) {
        if (inet_pton(AF_INET, argv[1], &seraddr.sin_addr) <= 0){
        	fprintf(stderr, "tcp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }

        if (inet_pton(AF_INET, argv[1], &udpsaddr.sin_addr) <= 0){
        	fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }
        
        seraddr.sin_port = htons(atoi(argv[2]));
        udpsaddr.sin_port = htons(atoi(argv[3]));
    } else if (argc == 3) {
        seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        udpsaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        seraddr.sin_port = htons(atoi(argv[1]));
        udpsaddr.sin_port = htons(atoi(argv[2]));
    } else {
        fprintf(stderr, "请输入正确的参数形式： cmd host port\n");
        exit(-1);
    }

    // tcp
    if (bind(listenfd, (struct sockaddr*) &seraddr, sizeof (seraddr)) < 0) {
        fprintf(stderr, "tcp 绑定地址失败，退出！\n");
        exit(-1);
    }

    // udp
	int res = bind(udpsockfd,(struct sockaddr*)&udpsaddr,sizeof(udpsaddr));
	if (res < 0) {
        fprintf(stderr, "udp 绑定地址失败，退出！\n");
        exit(-1);
    }

    listen(listenfd, 1024);

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < MAX_CLIENT; i++) {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    FD_SET(udpsockfd, &allset);

    while (1) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof (cliaddr);
            connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);
            if (connfd > -1) {
                char buffer[20];
                fprintf(stderr, "连接成功！客户端地址：%s,端口号：%d\n",
                        inet_ntop(AF_INET, &cliaddr.sin_addr, buffer, 20),
                        ntohs(cliaddr.sin_port));
            }
            if (connfd < 0) { 
                if (errno == EINTR)
                    continue;
                else {
                    fprintf(stderr, "accpt error, connfd：%d\n", connfd);
                    close(connfd);
                    continue;
                }
            }
            for (i = 0; i < MAX_CLIENT; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == MAX_CLIENT) {
                fprintf(stderr, "too many clients");
                exit(-1);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }

        if (FD_ISSET(udpsockfd, &rset)) {
            memset(bufudp,0,1024);
            int len=sizeof(udpcaddr);
            char buffer[20];
            char buffer2[1024];
            memset(buffer,0,20);
            memset(buffer2,0,1024);
            recvfrom(udpsockfd, buffer2,1024,0,(struct sockaddr*)&udpcaddr,&len);
            sprintf(bufudp, "recv from [%s:%d]: ", 
                        inet_ntop(AF_INET, &udpcaddr.sin_addr, buffer, 20),
                        ntohs(udpcaddr.sin_port));
            sprintf(bufudp + strlen(bufudp), "%s", buffer2);
            printf("%s\n",bufudp);
            // if(strncmp(buff,"end",3)==0)
            // {
            //     break;
            // }
            sendto(udpsockfd,bufudp,1024,0,(struct sockaddr*)&udpcaddr,sizeof(udpcaddr));
        }

        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd,& rset)) {
                if ((n = read(sockfd, buf, 1024)) == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else if(n > 0) {
                    if (write(sockfd, buf, n) < 0) {
                        fprintf(stderr, "write error\n");
                        exit(1);
                    }
                    memset(buf,0,1024);
                } else if(n < 0) {
                    fprintf(stderr, "read error!\n");
                    exit(-1); //read error
                }
                
                if (--nready <= 0)
                    break;
            }
        }
    }

    return 0;
}

void str_echo(int socket) {
    ssize_t n;
    char buf[1024 + 1];
    memset(buf, 0, 1024 + 1);
again:

    while ((n = read(socket, buf, 1024)) > 0) {
       if (write(socket, buf, n) < 0) {
        fprintf(stderr, "write error\n");
        return;
       }
    }
    if (n < 0 && errno == EINTR)
        goto again;
    if (n < 0) {
        fprintf(stderr, "read error\n");
        return;
    }
}



