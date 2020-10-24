#ifndef _MYLOG_H_
#define _MYLOG_H_

#include <stdlib.h>
#include <time.h>     /* time_t */
#include <sys/time.h> /* struct timeval */
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

#define LOG_BUF_SIZE 1024
#define LOG_CONF "conf"
#define LOG_FILE_SUFFIX ".log"

//异步日志类，创建一个线程把日志写入文件
class Log
{
public:
    //Log();
    ~Log();
    void initLog();

    static Log *instance();
    static void outputLog(Log *log, std::string msg);
    static void *logging(void *args); //线程创建需要静态成员函数
    void join();

protected:
    Log();

private:
    static Log *instance_;

private:
    char buf1[LOG_BUF_SIZE];
    char buf2[LOG_BUF_SIZE];
    char *toWrite;        //待写入文件的缓冲区
    char *toRecv;         //暂时没有用到
    char *logFileName; //日志文件名
    char logRootPath[1024];
    pthread_mutex_t logMutex;
    pthread_cond_t logCond;
    bool isStoredToWrite; //缓冲区是否填好日志
    FILE *logFilePtr;     //打开日志文件的流

    pthread_t logThread;

    //时间处理
    time_t now;
    struct tm tm1;
    struct tm *tmptr;
    struct timeval tv;
};

#endif
