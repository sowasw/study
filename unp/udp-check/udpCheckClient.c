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

int timeout = 50000;
int nready = 0; 
ssize_t n = 0;
fd_set rset, allset;
char bufudp[MAX_BUF_LEN];
socklen_t socklen;
struct timeval tv;
int udpsockfd;
struct sockaddr_in udpsaddr,udpcaddr;
string localip;
in_port_t localport;

int check_is_Nat();
int check_is_Full_Cone_NAT();
int check_is_Symmetric_NAT();
int check_is_Restricted_Cone_NAT();  //Port Restricted Cone NAT

//void str_echo(int socket);//回射程序，处理完套接字才返回，这里没有用到

int main(int argc, char** argv) {
    // int nready; 
    // ssize_t n;
    // fd_set rset, allset;
    // char bufudp[MAX_BUF_LEN];
    // socklen_t socklen;

    // udp
    udpsockfd = socket(AF_INET,SOCK_DGRAM,0);
    memset(&udpsaddr,0,sizeof(udpsaddr));
    memset(&udpcaddr,0,sizeof(udpcaddr));
    udpsaddr.sin_family = AF_INET;
    udpcaddr.sin_family = AF_INET;

    // udpsaddr.sin_port = htons(6000);
	// udpsaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (inet_pton(AF_INET, "127.0.0.1", &udpcaddr.sin_addr) <= 0){
        fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        exit(-1);
   	}
    if (inet_pton(AF_INET, "127.0.0.1", &udpsaddr.sin_addr) <= 0){
        fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        exit(-1);
   	}

    udpcaddr.sin_port = htons(CLI_PORT_UDP);
    udpsaddr.sin_port = htons(SERV_PORT_UDP);

    if (argc == 1) {
        // udpcaddr.sin_port = htons(CLI_PORT_UDP);
        // udpsaddr.sin_port = htons(SERV_PORT_UDP);
    } else if (argc == 3) {
        if (inet_pton(AF_INET, argv[1], &udpsaddr.sin_addr) <= 0){
        	fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }
        udpsaddr.sin_port = htons(atoi(argv[2]));
    } else if (argc == 5) {
        if (inet_pton(AF_INET, argv[1], &udpcaddr.sin_addr) <= 0){
        	fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }
        udpcaddr.sin_port = htons(atoi(argv[2]));

        // printf("local: [%s:%u,%d,%s]\n", argv[1], udpcaddr.sin_port, atoi(argv[2]), argv[2]);

        if (inet_pton(AF_INET, argv[3], &udpsaddr.sin_addr) <= 0){
        	fprintf(stderr, "udp 字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	    }
        udpsaddr.sin_port = htons(atoi(argv[4]));
    } else {
        fprintf(stderr, "请输入正确的参数形式： cmd localhost localport serverhost serverport\n");
        exit(-1);
    }

    // udp
	int res = bind(udpsockfd,(struct sockaddr*)&udpcaddr,sizeof(udpcaddr));
    char buftmp[20];
    localip = inet_ntop(AF_INET, &udpcaddr.sin_addr, buftmp, 20);
    localport = ntohs(udpcaddr.sin_port);
    printf("local: [%s:%d]\n", localip.c_str(), localport);

	if (res < 0) {
        fprintf(stderr, "udp 绑定地址失败，退出！\n");
        exit(-1);
    }

    FD_ZERO(&allset);
    FD_SET(udpsockfd, &allset);

    int ret = check_is_Nat();
    if (ret ) {
        return 0;
    } else {

    }

    return 0;
}

int check_is_Nat()
{
    int ntry = 0;
    while (1) {
        memset(bufudp,0,MAX_BUF_LEN);
        bufudp[0] = '1';
        // bufudp[0] = '2';

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout - (tv.tv_sec * 1000)) * 1000;//单位为微秒，不是毫秒、纳秒

        rset = allset;
        sendto(udpsockfd,bufudp,strlen(bufudp),0,(struct sockaddr*)&udpsaddr,(socklen_t)sizeof(udpsaddr));
        nready = select(udpsockfd + 1, &rset, NULL, NULL, &tv);

        if(nready > 0) {
            if (FD_ISSET(udpsockfd, &rset)) {
                char buffer[20];
                char buffer2[MAX_BUF_LEN+100];
                memset(buffer,0,20);
                memset(buffer2,0,MAX_BUF_LEN +100);
                memset(bufudp,0,MAX_BUF_LEN);
                recvfrom(udpsockfd, bufudp,MAX_BUF_LEN,0,(struct sockaddr*)&udpsaddr,&socklen);

                sprintf(buffer2, "FROM[%s:%d], MSG: ", 
                            inet_ntop(AF_INET, &udpsaddr.sin_addr, buffer, 20),
                            ntohs(udpsaddr.sin_port));
                sprintf(buffer2 + strlen(buffer2), "%s", bufudp);
                printf("%s\n",buffer2);

                string str = bufudp;
                string ip = str.substr(0,str.find(":"));
                string port = str.substr(str.find(":") + 1);
                in_port_t tmpport = (atoi(port.c_str()));

                printf("local: [%s:%d], server back local:[%s:%d]", localip.c_str(), localport, ip.c_str(), tmpport);
                if (ip == localip && tmpport == localport ) {
                    printf("NOT IN NAT\n");
                    
                    return 1;
                } else {
                    printf("would check go ahead\n");
                    return 0;
                }
            }

        printf("nready: %d, sec: %ld, usec: %ld\n",nready, tv.tv_sec, tv.tv_usec);
        } else {
            if (ntry++ >= 10) {
                printf("can not send/recv UDP data\n");
                exit(0);
            }
        }
    }
}
int check_is_Full_Cone_NAT()
{
    return 0;
}
int check_is_Symmetric_NAT()
{
return 0;
}
int check_is_Restricted_Cone_NAT()  //Port Restricted Cone NAT
{
return 0;
}





