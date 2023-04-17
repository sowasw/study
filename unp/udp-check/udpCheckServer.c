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

#include <string>
using namespace std;

//#define MAX_CLIENT FD_SETSIZE
#define MAX_CLIENT 1010
#define MAX_BUF_LEN 1024

#define SERV_PORT 9999
#define SERV_PORT_UDP 9998
#define CLI_PORT_UDP 9997


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

    struct sockaddr_in tmpaddr;
    memset(&tmpaddr,0,sizeof(tmpaddr));
    tmpaddr.sin_family = AF_INET;
    tmpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tmpaddr.sin_port = htons(43562);
    int tmpsockfd = socket(AF_INET,SOCK_DGRAM,0);
    int res1 = bind(tmpsockfd,(struct sockaddr*)&tmpaddr,sizeof(tmpaddr));
    if (res1 < 0) {
        fprintf(stderr, "udp 绑定地址失败，退出！\n");
        exit(-1);
    }

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
            int len=sizeof(udpcaddr);
            char buffer[20];
            char buffer2[MAX_BUF_LEN+100];
            memset(buffer,0,20);
            memset(buffer2,0,MAX_BUF_LEN +100);
            memset(bufudp,0,MAX_BUF_LEN);
            recvfrom(udpsockfd, bufudp,MAX_BUF_LEN,0,(struct sockaddr*)&udpcaddr,(socklen_t *)&len);
            sprintf(buffer2, "recv from [%s:%d]: ", 
                        inet_ntop(AF_INET, &udpcaddr.sin_addr, buffer, 20),
                        ntohs(udpcaddr.sin_port));
            sprintf(buffer2 + strlen(buffer2), "%s", bufudp);
            printf("%s\n",buffer2);

            char bufsend[MAX_BUF_LEN];
            memset(bufsend,0,MAX_BUF_LEN);

            if (bufudp[0] == '1') {
                sprintf(bufsend, "%s:%d``", buffer, ntohs(udpcaddr.sin_port));
                sendto(udpsockfd,bufsend,strlen(bufsend),0,(struct sockaddr*)&udpcaddr,(socklen_t)sizeof(udpcaddr));
            } else if (bufudp[0] == '2') {
                sprintf(bufsend, "%s:%d``", buffer, ntohs(udpcaddr.sin_port));
                sendto(tmpsockfd,bufsend,strlen(bufsend),0,(struct sockaddr*)&udpcaddr,(socklen_t)sizeof(udpcaddr));
            }
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




