#include "wxl_tcp.h"

int wxl_tcp_listen(int argc, char **argv){
	int listenfd;
	int ret;
	
	socklen_t clilen;
	//char address[ADDR_LEN], host[PORT_LEN];
	
	struct sockaddr_in srvaddr;
	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	//bzero(&srvaddr, sizeof (srvaddr));
	srvaddr.sin_family = AF_INET;
	
	if (argc == 1) {
		srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY
		srvaddr.sin_port = htons(SRV_PORT);//9999
	} else if (argc == 3) {
		ret = inet_pton(AF_INET, argv[1], &srvaddr.sin_addr);
		if (ret < 0) {
			printf("inet_pton error!\n");
			exit(-1);
		} else if (ret == 0) {//无效的格式
			printf("Invalid format, inet_pton error!\n");
			exit(-1);
		}
		srvaddr.sin_port = htons(atoi(argv[2]));//
	} else {
		printf("Invalid command line arguments!\n");
        exit(-1);
	}
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("socket error!\n");
		exit(-1);
	}
	ret = bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	if (ret < 0) {
		printf("bind error!\n");
		exit(-1);
	}
	ret = listen(listenfd, MAX_LISTENQ);
	if (ret < 0) {
		printf("listen error!\n");
		exit(-1);
	}

	return listenfd;
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
		printf ("> SIGCHLD <  child %d terminated\n", pid);
	}
	return;
}

void sig_pipe(int signo){
	printf("> SIGPIPE <\n");
}