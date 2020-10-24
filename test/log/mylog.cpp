#include "mylog.h"

char *getLogPath(char *path, const char *conf);

Log *Log::instance_ = NULL;

Log *Log::instance()
{
    if (instance_ == NULL)
        instance_ = new Log;
    return instance_;
}

Log::Log()
{
    ::memset(buf1, 0, LOG_BUF_SIZE);
    ::memset(buf2, 0, LOG_BUF_SIZE);
    toRecv = buf1;
    toWrite = buf1;
    isStoredToWrite = false;
    tmptr = &tm1;

    //self = this;
    getLogPath(logRootPath, LOG_CONF);
    logFileName = logRootPath + ::strlen(logRootPath);

    now = ::time(NULL);
    tmptr = ::localtime(&now);
    ::strftime(logFileName, 1024 - ::strlen(logRootPath), "%F %T", tmptr); //2019-11-06 17:53:01
    ::strcat(logFileName, LOG_FILE_SUFFIX);
    logFileName = logRootPath;
    logFilePtr = ::fopen(logFileName, "a");

    ::pthread_mutex_init(&logMutex, NULL);
    ::pthread_cond_init(&logCond, NULL);
    ::pthread_create(&logThread, NULL, &logging, this);
}

Log::~Log()
{
    ::fclose(logFilePtr);
}

void Log::initLog()
{
}

void Log::outputLog(Log *logToWrite, std::string msg)
{
    //Log *log = static_cast<Log *>(logToWrite);
    Log *log = logToWrite;
    ::pthread_mutex_lock(&log->logMutex);
    while (log->isStoredToWrite)
        ::pthread_cond_wait(&log->logCond, &log->logMutex);

    ::gettimeofday(&log->tv, NULL);
    log->tmptr = ::localtime(&log->tv.tv_sec);
    ::strftime(log->toWrite, LOG_BUF_SIZE, "%F %T", log->tmptr); //2019-11-06 17:53:01
    char *u_sec = log->toWrite + ::strlen(log->toWrite);
    ::sprintf(u_sec, ".%ld %s", log->tv.tv_usec, msg.c_str()); //2019-11-07 17:17:49.245972 + msg.c_str()

    log->isStoredToWrite = true;

    ::pthread_mutex_unlock(&log->logMutex);
}

void *Log::logging(void *args)
{
    //Log *log = static_cast<Log *>(args);
    Log *log = (Log *)args;
    //char *tmp;
    //log->initLog();
    while (true)
    {
        ::pthread_mutex_lock(&log->logMutex);
        if (log->isStoredToWrite)
        {
            ::fputs(log->toWrite, log->logFilePtr); //使用标准库IO函数需要捕捉信号以冲洗流
            //::fflush(NULL);

            //::write(::fileno(log->logFilePtr), log->toWrite, ::strlen(log->toWrite));
            log->isStoredToWrite = false;
        }
        ::pthread_mutex_unlock(&log->logMutex);
        ::pthread_cond_signal(&log->logCond);
    }
}

void Log::join()
{
    pthread_join(logThread, NULL);
}

char *getLogPath(char *path, const char *conf)
{
    FILE *fp = ::fopen(conf, "r");
    if (!fp)
        ::strcmp(path, "");
    char buf[1024];
    while (::fgets(buf, 1024, fp))
    {
        if (::strcmp(buf, "logdir\n") == 0)
        {
            if (!::fgets(path, 1024, fp))
                ::exit(-1);
            path[::strlen(path) - 1] = 0;
        }
    }
    ::fclose(fp);
    return path;
}
