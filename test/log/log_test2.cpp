#include <iostream>
#include <thread>
#include <signal.h>

#include "wexlog.h"

void sig_int(int sig)
{
	exit(1);
}

void *writeLogs(void *args);

int main(int argc, char **argv)
{
	int nthreads = 800;
	pthread_t tid[nthreads];

	signal(SIGINT, sig_int); //捕捉信号以冲洗流

	Log::initLog("conf");
	Log::outputLog("here\n");
	//writeLogs((void *)0);

	for (int i = 0; i < nthreads; i++)
		pthread_create(&tid[i], NULL, writeLogs, (void *)((long)i));

	for (int i = 0; i < nthreads; i++)
		pthread_join(tid[i], NULL);

	Log::join();

	exit(0);
}

void *writeLogs(void *args)
{
	int n = (long)args;
	//long n = 20;
	char buf[1024];
	snprintf(buf, 1024, "thread %d write logs", n);
	enum log_level lel = (enum log_level)((int)LOG_TRACE + n % 8);
	while (true)
	{
		//Log::outputLog(buf);
		Log::outputLog(lel, "%s in file: %s at line: %d\n", buf, __FILE__, __LINE__);
		//sleep(1);
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000)); //睡眠1-1000毫秒
	}
}
