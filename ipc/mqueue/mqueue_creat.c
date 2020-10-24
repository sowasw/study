/*
 *创建消息队列文件，在/dev/mqueue目录下
 */


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
	int n, flags;
	mqd_t mqd;
	mode_t mode = S_IRUSR | S_IWUSR;
	char mqname[1024];
	if(argc == 1)
		strcpy(mqname, MQ_NAME);
	else if(argc == 2)
		strcpy(mqname, argv[1]);
	else {
		printf("please input mqueue name in arg[1]\n");
		exit(0);
	}
	
	flags = O_RDWR | O_CREAT;
	
	mqd = mq_open(mqname, flags, mode, NULL);//创建的消息队列在/dev/queue目录下
	if(mqd < 0)
		printf("mqd open or creat error\n");
		
	printf("mqueue name: %s, mqd : %d created!\n",mqname, mqd);
	
	int ret;
	//ret = mq_close(mqd);
	while((ret = mq_close(mqd)) < 0)
		printf("mqueue name: %s, mqd : %d close error!\n",mqname, mqd);
	
	printf("mqueue name: %s, mqd : %d closed!\n",mqname, mqd);
		
	//mq_unlink(mqname);
	
	exit (0);
}


