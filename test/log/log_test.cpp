#include <iostream>
#include <signal.h>

#include "mylog.h"

void sig_int(int sig)
{
	exit(1);
}

void *writeLogs(void *args);

int main(int argc, char **argv)
{
	pthread_t tid;

	signal(SIGINT, sig_int);//捕捉信号以冲洗流
	Log *log = Log::instance();
	Log::outputLog(log, "here\n");
	
	pthread_create(&tid, NULL, writeLogs, log);

	
	pthread_join(tid, NULL);
	log->join();

	exit(0);
}

void *writeLogs(void *args){
	Log *log = (Log *)args;
	while(true){
		Log::outputLog(log, "writeLogs!\n");
		sleep(1);
	}
}
