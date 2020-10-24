#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define MAXLINE 1024


int main(int argc, char **argv){
	int fd[2];
	char buf0[MAXLINE];
	char buf1[MAXLINE];
	int n0 = 0, n1 = 0;
	
	socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);
	
	//两个unix套接字都可以读写，全双工
	write(fd[0], "fd0\n", 4);
	write(fd[1], "fd1\n", 4);
	n0 = read(fd[0], buf0, MAXLINE);
	write(1, buf0, n0);
	
	n1 = read(fd[1], buf1, MAXLINE);
	write(1, buf1, n1);
	
	//回射标准输入
/*	while(1){*/
/*		n0 = read(0, buf0, MAXLINE);*/
/*		write(fd[0], buf0, n0);*/
/*		*/
/*		n1 = read(fd[1], buf1, MAXLINE);*/
/*		write(1, buf1, n1);*/
/*	}*/


	//父子进程通信, 回射标准输入
	//子进程读stdin，发送给父进程，
	//父进程读子进程发送的数据，回射给子进程，
	//子进程读父进程发送的数据，写到stdout
	if(fork() == 0){	//child
		close(fd[1]);
		
		while( (n0 = read(0, buf0, MAXLINE)) > 0){
			write(fd[0], buf0, n0);
			n1 = read(fd[0], buf1, MAXLINE);
			write(1, buf1, n1);
		}
		exit(0);
	}else{				//parent
		close(fd[0]);
		//sleep(1);
		
		while( (n1 = read(fd[1], buf1, MAXLINE)) > 0){
			write(fd[1], buf1, n1);
		}
	}
	
	
	exit (0);
}
