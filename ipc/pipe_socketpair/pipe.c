#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/un.h>
//#include <sys/socket.h>

#define MAXLINE 1024


int main(int argc, char **argv){
	int fd1[2], fd2[2];
	char buf0[MAXLINE];
	char buf1[MAXLINE];
	int n0 = 0, n1 = 0;
	
	//创建两个管道
	pipe(fd1);
	pipe(fd2);
	
	//pipe创建的管道fd[0]为读而打开,fd[1]为写而打开
	write(fd1[1], "fd1[2]\n", 7);
	n0 = read(fd1[0], buf0, MAXLINE);
	write(1, buf0, n0);
		
	write(fd2[1], "fd2[2]\n", 7);
	n1 = read(fd2[0], buf1, MAXLINE);
	write(1, buf1, n1);
	
	//父子进程通信, 回射标准输入
	//子进程读stdin，发送给父进程，
	//父进程读子进程发送的数据，回射给子进程，
	//子进程读父进程发送的数据，写到stdout
	if(fork() == 0){	//child
		close(fd1[1]);
		close(fd2[0]);
		
		while( (n0 = read(0, buf0, MAXLINE)) > 0){
		
			write(fd2[1], buf0, n0);
			n1 = read(fd1[0], buf1, MAXLINE);
			
			write(1, buf1, n1);
		}
		exit(0);
	}else{				//parent
		close(fd1[0]);
		close(fd2[1]);
		//close(0);
		
		while( (n1 = read(fd2[0], buf1, MAXLINE)) > 0){
			write(fd1[1], buf1, n1);
		}
	}
	
	
	exit (0);
}
