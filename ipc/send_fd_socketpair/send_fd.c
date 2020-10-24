/*
 * 子进程向父进程传递文件描述符
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <fcntl.h>

#define MAXLINE 1024
#define FILENAME "file"

int CONTROL_LEN = CMSG_LEN(sizeof(int));
ssize_t send_fd(int fd, int sendfd);
ssize_t read_fd(int fd, int *recvfd);

int main(int argc, char **argv){
	int sockfd[2], fd1, fd2;
	pid_t childpid;
	char buf[MAXLINE];
	char buf1[MAXLINE];
	int n0 = 0, n1 = 0;
	
	socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
	

	//子进程向父进程传递文件描述符
	if((childpid = fork()) == 0){	//child
		close(sockfd[1]);
		printf("child: \n");
		
		fd1 = open(FILENAME, O_RDWR | O_APPEND);
		n0 = read(fd1, buf, 1);
		write(1, buf, n0);//子进程先读一个字符并打印到标准输出
		
		//send fd
		send_fd(sockfd[0], fd1);
		close(fd1);
		
		exit(0);
	}else{				//parent
		close(sockfd[0]);
		sleep(3);//让子进程先运行
		printf("\nparent: \n");
		
		//read fd
		read_fd(sockfd[1], &fd2);
		
		//父进程读剩下的内容打印到标准输出
		while((n1 = read(fd2, buf1, MAXLINE)) > 0){
			write(1, buf1, n1);
		}
	}
	
	exit (0);
}

ssize_t send_fd(int fd, int sendfd){
	struct iovec iov[1];
	struct msghdr msg;
	char buf[1];
	
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	
	struct cmsghdr cm;
	cm.cmsg_len = CONTROL_LEN;
	cm.cmsg_level = SOL_SOCKET;
	cm.cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(&cm) = sendfd;
	msg.msg_control = &cm;
	msg.msg_controllen = CONTROL_LEN;
	
	ssize_t n = sendmsg(fd, &msg, 0);
	return n;
}

ssize_t read_fd(int fd, int *recvfd){
	struct iovec iov[1];     
	struct msghdr msg;
	char buf[1];
	
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	
	struct cmsghdr cm;
/*	cm.cmsg_len = CONTROL_LEN;*/
/*	cm.cmsg_level = SOL_SOCKET;*/
/*	cm.cmsg_type = SCM_RIGHTS;*/
/*	*(int*)CMSG_DATA(&cm) = sendfd;*/
	msg.msg_control = &cm;
	msg.msg_controllen = CONTROL_LEN;
	
	ssize_t n = recvmsg(fd, &msg, 0);
	*recvfd = *(int*)CMSG_DATA(&cm);
	return n;
}
