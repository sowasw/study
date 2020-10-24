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
#include <stdlib.h>
#include <time.h>


#define max(x, y)  (x) > (y)? (x):(y)


/*
 * I/O复用的回射客户端，select监听套接字和stdin
 * 命令行第一个参数指定服务器IP，第二个参数指定端口号，
 * 如果没有命令行参数，则默认指定"127.0.0.1"，端口号9999
 */
 
 
void str_cli(FILE* fp, int sockfd);

int main(int argc, char** argv) {

    int connfd;
    struct sockaddr_in seraddr;
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd < 0) {
        fprintf(stderr, "套接字创建失败，退出！socket error!\n");
        exit(-1);
    }

    memset(&seraddr, 0, sizeof (seraddr));
    seraddr.sin_family = AF_INET;
    if(argc == 1){
    	if (inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr) <= 0) {
        	fprintf(stderr, "字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   		}
   		seraddr.sin_port = htons(9999);
    } else if(argc == 3) {
    	if (inet_pton(AF_INET, argv[1], &seraddr.sin_addr) <= 0) {
        	fprintf(stderr, "字节序转换失败！inet_pton error!\n");
        	exit(-1);
   		}
   		seraddr.sin_port = htons(atoi(argv[2]));
    } else {
    	fprintf(stderr, "不正确的命令行参数格式!\n");
        exit(-1);
    }

    if (connect(connfd, (struct sockaddr*) &seraddr, sizeof (seraddr)) < 0) {
        fprintf(stderr, "连接失败！connect error!\n");
        exit(-1);
    } else
        printf("连接成功!\n");

//    FILE *fp;
//    fp = fopen("file.txt", "r"); //上步已经到文件末尾，重新打开
//        printf("从文件中读取!\n");
//        str_cli(fp, connfd); //从文件中读取

    printf("从标准输入中读取!\n");
    str_cli(stdin, connfd); //从标准输入中读取

    exit(0);
}

/**
 * FD_ZERO(fd_set *fdset);将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化，
 * 如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
　　FD_SET(fd_set *fdset);用于在文件描述符集合中增加一个新的文件描述符。
　　FD_CLR(fd_set *fdset);用于在文件描述符集合中删除一个文件描述符。
　　FD_ISSET(int fd,fd_set *fdset);用于测试指定的文件描述符是否在该集合中。
 * @param fp
 * @param sockfd
 */
void str_cli(FILE* fp, int sockfd) {

    //V3,批量输入,针对缓冲区操作，不以文本行为中心,管道充分利用
    int maxfdp1, stdineof;
    fd_set rset;
    int n;
    struct timeval tv;

    char buf[1024];
    memset(buf, 0, 1024);

    stdineof = 0;
    FD_ZERO(&rset);
    while (1) {
        tv.tv_sec =0;
        tv.tv_usec = 10000000;//单位为微秒，不是毫秒、纳秒

        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, &tv); //select
        //printf("   服务中1...\n");

        if (FD_ISSET(sockfd, &rset)) {
            //            printf("接收中...!sockfd:%d\n", sockfd);
            if ((n = read(sockfd, buf, 1024)) == 0) {//从套接字中接收
                printf("接收0字节! sockfd: %d\n", sockfd);
                if (stdineof == 1) {
                    fprintf(stderr, "normal termination!（正常退出）\n");
                    return; //normal termination
                } else {
                    fprintf(stderr, "服务器关闭，服务结束!\n");
                    return;
                }
            }
            if(write(fileno(stdout), buf, n) < 0) //写入标准输出
            {
                fprintf(stderr, "write error! exit.\n");
				return;
            }
        }
        if (FD_ISSET(fileno(fp), &rset)) {
            //            printf("接收中...!fd:%d\n", fileno(fp));
            if ((n = read(fileno(fp), buf, 1024)) == 0) {//从文件流描述符读数据，这里调用后是标准输入

                fprintf(stderr, "读入0字节!关闭一半！\n");
                stdineof = 1;
                shutdown(sockfd, SHUT_WR); //发送完，关闭一半，
                FD_CLR(fileno(fp), &rset); //FD_CLR(fd_set *fdset);用于在文件描述符集合中删除一个文件描述符。
                //此句不能少
                continue;

            }
            if(n < 0){
				    fprintf(stderr, "read the FILE error! exit.\n");
				    break;
			}
			
			
            if(write(sockfd, buf, n) < 0) {
                 fprintf(stderr, "write to the socket error! exit.\n");
                 return;
            }
            //                        printf("发送%d!\n", n);
        }
        //printf("   服务中2...\n");
    }

}


