#ifndef _wxl_tcp_h_
#define _wxl_tcp_h_

#include "wxl_head.h"

int wxl_tcp_listen(int argc, char **argv);
int wxl_tcp_accept(int listenfd, struct sockaddr_in* cliaddr, socklen_t clilen);
int wxl_tcp_connect(int sockfd, struct sockaddr_in* srvaddr, socklen_t srvlen);

typedef void SIGFUNC(int);
SIGFUNC* mysignal(int signo, SIGFUNC* func);
void sig_chld(int signo);
void sig_pipe(int signo);


#endif//_wxl_tcp_h_
