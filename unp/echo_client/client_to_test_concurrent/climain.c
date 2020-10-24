/*
 *回射服务器的多进程客户端，用于测试并发服务器
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>//inet_pton
#include <fcntl.h>  // for nonblocking 
#include <netdb.h>
#include <signal.h>
#include <sys/stat.h> // for S_xxx file mode constants 
#include <sys/uio.h>  // for iovec{} and readv/writev 
#include <sys/wait.h>
#include <sys/un.h>

#define MAXLINE 1024

#define SRV_ADDR "127.0.0.1"

#define SRV_PORT 9999

//子进程数
//linux进程默认最多打开1024个描述符
//服务器端需设置环境变量或ulimit命令改变描述符限制
#define NCHILD 1000

typedef void SIGFUNC(int);
SIGFUNC* mysignal(int signo, SIGFUNC* func);
void sig_chld(int signo);
void sig_int(int signo);

int main(int argc, char** argv){
	char sendbuf[MAXLINE];
	char recvbuf[MAXLINE];
	int i, nread;
	int sockfd;
	char host[16];
	int port;
	int nchild = NCHILD;
	struct sockaddr_in srvaddr;
	
	mysignal(SIGCHLD, sig_chld);
	mysignal(SIGINT, sig_int);
	
	if(argc == 1){
	    snprintf(host, sizeof(host), "%s", SRV_ADDR);
	    port = 9999;
	} else if(argc == 2){
	    snprintf(host, sizeof(host), "%s", SRV_ADDR);
	    port = 9999;
	    nchild = atoi(argv[1]);
	} else if(argc == 4){
	    snprintf(host, sizeof(host), "%s", argv[1]);
	    port = atoi(argv[2]);
	    nchild = atoi(argv[3]);
	    //printf("%s:%d\n", host, port);
	} else {
	    fprintf(stderr, "please input args like: cmd host port nprocess, or: cmd nprocess\n");
	    exit(0);
	}
	
	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	//bzero(&srvaddr, sizeof (srvaddr));
	srvaddr.sin_family = AF_INET;
	//srvaddr.sin_port = htons(SRV_PORT);
	//inet_pton(AF_INET, SRV_ADDR, &srvaddr.sin_addr);
	
	srvaddr.sin_port = htons(port);
	if(inet_pton(AF_INET, host, &srvaddr.sin_addr) <= 0){
        	fprintf(stderr, "字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	}
	
	
	for(i = 0; i < nchild; i++){
	    int childpid = fork();
		if(childpid == 0){
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0){
				fprintf(stderr, "connect error!\n");
				exit(1);
			}
			
			while(1){
			
				nread = read(0, sendbuf, MAXLINE);
				if(nread == 0){
				    fprintf(stderr, "read stdin EOF. exit the client.\n");
				    break;
				}
				else if(nread < 0){
				    fprintf(stderr, "read buf error!\n");
				    break;
				}
				    
				if(write(sockfd, sendbuf, nread) < 0){
				    fprintf(stderr, "write to the socket error!\n");
				    break;
				}
				
				nread = read(sockfd, recvbuf, MAXLINE);
				if(nread == 0){
				    fprintf(stderr, "read EOF. the server closed!\n");
				    break;
				}
				else if(nread < 0){
				    fprintf(stderr, "read the socket error!\n");
				    break;
				}
				if(write(1, recvbuf, nread) < 0){
				    fprintf(stderr, "write to stdout error!\n");
				    break;
				}
				//close(sockfd);
			}
			close(sockfd);
			exit(0);
		} else if(childpid < 0){
		    fprintf(stderr, "fork error!\n");
		}
	}
	
	while(1)
		pause();
}

SIGFUNC* mysignal(int signo, SIGFUNC* func){
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; //
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
	if(sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}

void sig_chld(int signo){
	pid_t	pid;
	int 	stat;
	//pid = wait(&stat);
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0){
		//printf ("> SIGCHLD <  child %d terminated\n", pid);
	}
	return;
}

void sig_int(int signo){
    exit(1);
}
