/*
 *回射服务器，多进程并发
 *
 */
#define _XOPEN_SOURCE 501 //for ptsname,grantpt,unlockpt

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h> 
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> /* inet(3) functions */
#include <errno.h> /* inet(3) functions */
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>


#define MAXLINE 1024

#define SRV_ADDR "127.0.0.1"
#define SRV_PORT 9998

typedef void SIGFUNC(int);
SIGFUNC* mysignal(int signo, SIGFUNC* func);
void sig_chld(int signo);
void master_ser(int mfd, int sockfd);

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
	    port = SRV_PORT;
	} else if(argc == 3){
	    snprintf(host, sizeof(host), "%s", argv[1]);
	    port = atoi(argv[2]);
	    //printf("%s:%d\n", host, port);
	} else {
	    fprintf(stderr, "please input args like: cmd host port\n");
	    exit(0);
	}
	
	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	srvaddr.sin_family = AF_INET;
	
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
	
	//signal(SIGCHLD, sig_chld);
	mysignal(SIGCHLD, sig_chld);
	
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
			if(setsid() < 0)
                    exit(1);
            //perror("setsid: \n");
            //printf("getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
			
			//signal(SIGCHLD, sig_chld);
			
            int mfd, sfd, pid;
            char pts_name[30];
            
            char sbuf[1024], rbuf[1024];
            int nreads, nreadr;
            
            //打开伪终端主设备
            //pfd = posix_openpt(O_RDWR);
            mfd = open("/dev/ptmx", O_RDWR);
            
            grantpt(mfd);
            unlockpt(mfd);
            
            //printf("ptsfd: %d\n", mfd);
            
            char *ptr =  ptsname(mfd);
            strncpy(pts_name, ptr, 30);
            pts_name[29] = '\0';
            
            //printf("ptsname: %s\n", pts_name);
            printf("getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
            
            //子进程打开伪终端从设备，执行登录程序，登录程序以伪终端从设备作为控制终端
            if((pid = fork()) == 0)
            {
                close(mfd);
                close(connfd);
                printf("pts: getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
                
                //创建会话，成为新进程组组长，之后没有控制终端
                if(setsid() < 0)
                    exit(1);
                //perror("setsid: \n");
                //printf("getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
                
                //打开伪终端从设备为控制终端
                sfd = open(pts_name, O_RDWR);
                
                //把伪终端从设备的描述符复制到stdin，stdout，stderr
                dup2(sfd, 0);
                dup2(sfd, 1);
                dup2(sfd, 2);
                
                //exec执行login程序
                //execl("/bin/login", "login", (char *)0);
                
                char *logargv[] = { "login", (char*)0 };
                execv("/bin/login", logargv);
                 
                close(sfd);
                return 0;
            } 
            
            //父进程读写伪终端主设备
            else 
            { 
                
                master_ser(mfd, connfd);
                
                close(connfd);
                close(mfd);
                
                //sleep(1);
                //waitpid(-1, NULL, WNOHANG);
                int status;
                //wait(&status);
			    exit(0);
                     
            }

		}else//parent
			close(connfd);
	}
	
	close(listenfd);
	exit(0);
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
		//printf ("child %d terminated\n", pid);
	}
	return;
}

void master_ser(int mfd, int sockfd) {
    int maxfdp1, stdineof;
    fd_set rset;
    int n, n2;
    struct timeval tv;

    char buf[1024], buf2[1024];
    memset(buf, 0, 1024);
    memset(buf2, 0, 1024);
    
/*    dup2(mfd, 0);*/
/*    dup2(mfd, 1);*/
/*    dup2(mfd, 2);*/
    //int infd = sockfd;
    int stdinfd = mfd;
    int stdoutfd = mfd;

    stdineof = 0;
    FD_ZERO(&rset);
    
    while (1) {
        tv.tv_sec =0;
        tv.tv_usec = 10;//单位为微秒，不是毫秒、纳秒
        
        FD_SET(stdinfd, &rset);
        FD_SET(sockfd, &rset);
        
        maxfdp1 = stdinfd > sockfd ? stdinfd + 1 : sockfd + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL); //select

        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, buf, 1024)) > 0) {//从套接字中接收
                 
                if(write(stdoutfd, buf, n) < 0) 
                {
                    fprintf(stderr, "write stdout error! exit.\n");
				    return;
                }
            }
            if(n == 0){
				    fprintf(stderr, "socket closed.\n");
				    break;
			}
        }
        if (FD_ISSET(stdinfd, &rset)) {
            if ((n2 = read(stdinfd, buf2, 1024)) > 0) {

                if(write(sockfd, buf2, n2) < 0) {
                    fprintf(stderr, "write to the socket error! exit.\n");
                    return;
                }

            }
            if(n2 < 0){
				    fprintf(stderr, "read pty master error! exit.\n");
				    break;
			}
			
        }
        
    }// while(1)

}
