#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <mqueue.h>

#define MAXLINE 1024
#define MQ_NAME "/mqfile"


int main(int argc, char **argv){
	int flags;
	mqd_t mqd;
	mode_t mode = S_IRUSR | S_IWUSR;
	char mqname[MAXLINE];
	int ret;
	if(argc == 1)
		strcpy(mqname, MQ_NAME);
	else if(argc == 2)
		strcpy(mqname, argv[1]);
	else {
		printf("please input mqueue name in arg[1]\n");
		exit(0);
	}
	
	flags = O_RDWR | O_CREAT;
	
	mqd = mq_open(mqname, flags, mode, NULL);
	if(mqd < 0)
		printf("mqd open or creat error\n");
		
	printf("mqueue name: %s, mqd : %d created!\n",mqname, mqd);
	
	
	//获取消息队列信息，状态
	//要设置最大消息数，最大字节数是在创建时,通过第四个参数attr结构
	//mq_setattr函数只能改变mq_flags
	struct mq_attr attr;
	ret = mq_getattr(mqd, &attr);
	printf("max msgs = %ld\nmax bytes/msg = %ld\ncurrently = %ld\n",
				attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	

	//关闭消息队列
	if((ret = mq_close(mqd)) < 0){
		printf("mqueue name: %s, mqd : %d close error!\n",mqname, mqd);
		exit(-1);
	}
	
	printf("mqueue name: %s, mqd : %d closed!\n",mqname, mqd);
	
	///////
	//1
	//父子进程通信
	pid_t pid;
	pid = fork();
	if (pid == 0){//child	
		//send
		char sendbuf[MAXLINE];
		//char *sendbuf = (char*)calloc(MAXLINE, sizeof(char));
		memset(sendbuf, 0, MAXLINE);
		flags = O_WRONLY;
		
		mqd = mq_open(mqname, flags, mode, NULL);
		strcpy(sendbuf, "hello,world!\n");
		mq_send(mqd, sendbuf, strlen(sendbuf), 0);
		printf("child sended msg.\n");
		exit(0);
	}
	//parent
	//sleep(1);//让子进程先运行并发送消息
	
	//recv
	//char recvbuf[MAXLINE];
	char *recvbuf = (char*)calloc(attr.mq_msgsize, sizeof(char));
	memset(recvbuf, 0, MAXLINE);
	flags = O_RDONLY;
	
	mqd = mq_open(mqname, flags, mode, NULL);
	mq_receive(mqd, recvbuf, attr.mq_msgsize, NULL);
	printf("parent receive msg: \n    %s", recvbuf);
	
	//////
	//2
	//父子进程通信，
	//子进程读stdin发送给父进程，父进程回射，子进程写到stdout
	sleep(1);
	printf("\nparent echo child: (with \"<\")\n");
	int bufsize = attr.mq_msgsize;
	int n = 0;
	pid = fork();
	if (pid == 0){//child
		char *sendbuf = (char*)calloc(bufsize, sizeof(char));
		char *recvbuf1 = (char*)calloc(bufsize, sizeof(char));
		memset(sendbuf, 0, bufsize);
		memset(recvbuf1, 0, bufsize);
		flags = O_RDWR;
		
		mqd = mq_open(mqname, flags, mode, NULL);
		while((n = read(0, sendbuf, bufsize)) > 0){
			mq_send(mqd, sendbuf, n, 0);
			//sleep(30);
			int n1 = mq_receive(mqd, recvbuf1, bufsize, NULL);
			write(1, recvbuf1, n1);
		}
		exit(0);
	}
	//parent
	char *recvbuf1 = (char*)calloc(bufsize + 10, sizeof(char));
	memset(recvbuf1, 0, bufsize + 10);
	flags = O_RDWR;
	
	mqd = mq_open(mqname, flags, mode, NULL);
	//sleep(30);
	while((n = mq_receive(mqd, recvbuf1, bufsize, NULL)) > 0){
		*(recvbuf1 + n - 1) = '<';//给回射的一行后面加一个标志小于符号“<”
		*(recvbuf1 + n) = '\n';
		mq_send(mqd, recvbuf1, n + 1, 0);
	}

	
	exit (0);
}


