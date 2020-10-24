#ifndef _wxl_head_h_
#define _wxl_head_h_


#ifdef __cplusplus
extern "C" {
#endif

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
#include <pthread.h>
#include <signal.h> 
#include <sys/resource.h>/*void sig_int(int signo)*/
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/epoll.h>

#include <assert.h>


#define MAXLINE 1024
#define MAX_LISTENQ 1024
#define MAX_NCHILD 1024
#define MAX_THREADS 1024

#define SRV_IP_ADDRESS "127.0.0.1"
#define SRV_PORT 9999
#define ADDR_LEN 15
#define PORT_LEN 5

typedef char bool;//c
#define true 0x01
#define false 0x00

#ifdef __cplusplus
}
#endif

#endif//_wxl_head_h_
