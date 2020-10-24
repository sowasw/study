/*
 *回射服务器，多进程并发
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
/*#include <fcntl.h>  // for nonblocking */
/*#include <netdb.h>*/
#include <signal.h>
#include <sys/stat.h> // for S_xxx file mode constants 
#include <sys/wait.h>
/*#include <sys/uio.h>  // for iovec{} and readv/writev */
/*#include <sys/un.h>*/
#include <errno.h>

#define MAXLINE 1024

#define SRV_ADDR "127.0.0.1"
#define SRV_PORT 9999
//#define SRV_PORT 17377

typedef void SIGFUNC(int);

SIGFUNC* mysigal(int signo, SIGFUNC* func);
void sig_chld(int signo);

int main(int argc, char** argv){
	char buf[MAXLINE];
	int n;
	int connfd;
	int listenfd;
	socklen_t clilen;
	char host[16];
	int port;
	struct sockaddr_in srvaddr, cliaddr;
	
	if(argc == 1){
	    snprintf(host, sizeof(host), "%s", SRV_ADDR);
	    port = 9999;
	} else if(argc == 3){
	    snprintf(host, sizeof(host), "%s", argv[1]);
	    port = atoi(argv[2]);
	    //printf("%s:%d\n", host, port);
	} else {
	    fprintf(stderr, "please input args like: cmd host port\n");
	    exit(0);
	}
	
	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	//bzero(&srvaddr, sizeof (srvaddr));
	srvaddr.sin_family = AF_INET;
	//srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY
	//inet_pton(AF_INET, "127.0.0.1", &srvaddr.sin_addr);
	//srvaddr.sin_port = htons(SRV_PORT);//9999
	
	srvaddr.sin_port = htons(port);
	if(inet_pton(AF_INET, host, &srvaddr.sin_addr) <= 0){
        	fprintf(stderr, "字节序转换失败，或为无效的格式！inet_pton error!\n");
        	exit(-1);
   	}
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(listenfd, (struct sockaddr*) &srvaddr, sizeof (srvaddr)) < 0) {
        fprintf(stderr, "绑定地址失败，退出！\n");
        exit(-1);
    }
	listen(listenfd, 1024);
	
	mysigal(SIGCHLD, sig_chld);
	
	while(1){
		if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0){
			if(errno == EINTR)
				continue;
			else{
				fprintf(stderr, "accept error\n");
				exit(1);
			}
				
		}
		if(fork() == 0){//child
			close(listenfd);
			while(1){
				n = read(connfd, buf, MAXLINE);
				if(n == 0)//EOF
				    break;
				else if(n < 0){
				    fprintf(stderr, "read the socket error! close the server process.\n");
				    break;
				}
				//write(1, buf, n);
				if (write(connfd, buf, n) < 0){
				    fprintf(stderr, "write to the socket error! close the server process.\n");
				    break;
				}
			}
			
			close(connfd);
			exit(0);
			
		}else//parent
			close(connfd);
	}
	close(listenfd);
	exit(0);
}

SIGFUNC* mysigal(int signo, SIGFUNC* func){
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
		//printf ("child %d terminated\n", pid);
	}
	return;
}
