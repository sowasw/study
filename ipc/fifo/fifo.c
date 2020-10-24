#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 1024
#define FIFO_NAME1 "fifo.1"
#define FIFO_NAME2 "fifo.2"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main(int argc, char **argv){
	int wrfd, rdfd;
	char buf0[MAXLINE];
	char buf1[MAXLINE];
	int n0 = 0, n1 = 0;
	
	//创建两个命名管道
	mkfifo(FIFO_NAME1, FILE_MODE);
	mkfifo(FIFO_NAME2, FILE_MODE);
	//mkfifo创建的管道已隐含指定了O_CREAT | O_EXCL,如果已存在则创建失败
	
	
	//父子进程通信, 回射标准输入
	//子进程读stdin，发送给父进程，
	//父进程读子进程发送的数据，回射给子进程，
	//子进程读父进程发送的数据，写到stdout
	if(fork() == 0){	//child
		wrfd = open(FIFO_NAME1, O_WRONLY);//fifo是半双工，只能要么打开来读，要么打开来写
		rdfd = open(FIFO_NAME2, O_RDONLY);
		
		while( (n0 = read(0, buf0, MAXLINE)) > 0){
		
			write(wrfd, buf0, n0);
			n1 = read(rdfd, buf1, MAXLINE);
			
			write(1, buf1, n1);
		}
		exit(0);
	}else{				//parent
		rdfd = open(FIFO_NAME1, O_RDONLY);
		wrfd = open(FIFO_NAME2, O_WRONLY);
		//rdfd = open(FIFO_NAME1, O_RDONLY);
		//这里打开顺序不能乱，否则会阻塞进程
		//unpv2 p43
		//如果一个进程打开某个fifo来读，但是没有进程打开这个fifo来写，将阻塞进程
		
		while( (n1 = read(rdfd, buf1, MAXLINE)) > 0){
			write(wrfd, buf1, n1);
		}
	}
	
	unlink(FIFO_NAME1);//引用计数减1,
	unlink(FIFO_NAME2);
	
	
	exit (0);
}
