#include "wexlog.h"

#define LOG_FILE_SUFFIX ".log"
#define LOG_PREFIX_LENGTH 11
#define LOG_TIME_LENGTH 27

std::string getLogPath(const char *conf);
std::string getLogFileName();
FILE *openLogFile(const std::string &path, const std::string &file);

Log *Log::instance_ = NULL;

Log *Log::instance()
{
    if (instance_ == NULL)
    {
        instance_ = new Log(NULL);
    }
    return instance_;
}

Log *Log::instance(const char *conf)
{
    if (instance_ == NULL)
    {
        instance_ = new Log(conf);
    }
    return instance_;
}

void Log::initLog(const char *conf)
{
    instance_ = instance(conf);
}

Log::Log(const char *config)
{
    if (config == NULL)
        logFile = stderr;
    else
    {
        conf = config;
        logRootPath = getLogPath(conf.c_str());
        logFileName = getLogFileName();
        logFile = openLogFile(logRootPath, logFileName);
    }

    toStore = bufferQueue;
    toWrite = bufferQueue + 1;
    //isStoredToWrite = 0;

    wakeupfd = eventfd(0, 0);
    evcount = 0;

    ::pthread_mutex_init(&logMutex, NULL);
    //::pthread_cond_init(&logCond, NULL);
    ::pthread_create(&logThread, NULL, &logging, NULL);
}

Log::~Log()
{
}

void Log::outputLog(enum log_level lel, const char *fmt, ...)
{
    Log *log = instance();
    std::shared_ptr<std::string> msg_ptr = std::make_shared<std::string>();

    //获取可变参数长度，并分配缓冲区
    va_list vl;
    va_start(vl, fmt);
    int fmtLength = vsnprintf(NULL, 0, fmt, vl);
    va_end(vl);
    int prefLength = LOG_TIME_LENGTH + LOG_PREFIX_LENGTH;
    int length = fmtLength + prefLength + 10;

    if (msg_ptr->capacity() < length)
        msg_ptr->resize(length);

    //日志缓冲区
    char *buf = (char *)msg_ptr->data();

    //填充时间格式
    struct tm tm;
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    ::strftime(buf, LOG_TIME_LENGTH, "%F %T", &tm); //2020-04-02 11:44:23
    char *u_sec = buf + ::strlen(buf);
    ::sprintf(u_sec, ".%06ld ", tv.tv_usec); //2020-04-02 11:44:23.508266

    //填充日志格式
    switch (lel)
    {
    case LOG_INFO:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_INFO");
        break;
    case LOG_TRACE:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_TRAC");
        break;
    case LOG_DEBUG:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_DEBU");
        break;
    case LOG_WARNING:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_WARN");
        break;
    case LOG_ERROR:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_ERRO");
        break;
    case LOG_SYSERROR:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_SYSE");
        break;
    case LOG_FATAL:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_FATA");
        break;
    case LOG_CRITICAL:
        sprintf(buf + LOG_TIME_LENGTH, "[%s] ", "LOG_CRIT");
        break;

    default:
        break;
    }

    //填充可变参数
    va_list vl1;
    va_start(vl1, fmt);
    vsnprintf(buf + prefLength, fmtLength + 1, fmt, vl1);
    va_end(vl1);

    //发送消息到日志线程
    ::pthread_mutex_lock(&log->logMutex);
    log->toStore->push(msg_ptr);
    //log->isStoredToWrite = true;
    eventfd_write(log->wakeupfd, 1);
    ::pthread_mutex_unlock(&log->logMutex);
}

void Log::outputLog(const char *msg)
{
    Log *log = instance();
    std::shared_ptr<std::string> msg_ptr = std::make_shared<std::string>(msg);

    ::pthread_mutex_lock(&log->logMutex);
    log->toStore->push(msg_ptr);
    //log->isStoredToWrite = true;
    ::pthread_mutex_unlock(&log->logMutex);
}

void *Log::logging(void *args)
{
    Log *log = instance();
    while (true)
    {
        eventfd_read(log->wakeupfd, &log->evcount);
        ::pthread_mutex_lock(&log->logMutex);
        //if (log->isStoredToWrite)
        {
            log->tmp = log->toWrite;
            log->toWrite = log->toStore;
            log->toStore = log->tmp;
            // ::fputs(log->toWrite, log->logFilePtr); //使用标准库IO函数需要捕捉信号以冲洗流
            // //::fflush(NULL);

            // //::write(::fileno(log->logFilePtr), log->toWrite, ::strlen(log->toWrite));

            //log->isStoredToWrite = false;
        }
        ::pthread_mutex_unlock(&log->logMutex);

        int n = 0;
        while (!log->toWrite->empty())
        {
            std::shared_ptr<std::string> ssp = log->toWrite->front();
            //判断到文件结尾，新建文件
            if (fputs(ssp->c_str(), log->logFile) < 0)
            {
                if (log->logFile != stderr && feof(log->logFile))
                {
                    fflush(log->logFile);
                    fclose(log->logFile);
                    log->logFileName = getLogFileName();
                    log->logFile = openLogFile(log->logRootPath, log->logFileName);
                }
                continue;
            }

            fflush(log->logFile);
            log->toWrite->pop();

            ++n;
        }

        //printf("evcount: %ld, logoutput: %d\n", log->evcount, n);//debug
    }
}

void Log::join()
{
    pthread_join(instance()->logThread, NULL);
}

std::string getLogPath(const char *conf)
{
    if (conf[0] == '\0')
        return std::string("");

    FILE *fp = fopen(conf, "r");
    if (!fp)
        goto err;

    char buf[1024];
    char *path;
    while (path = fgets(buf, 1024, fp))
    {
        if (strncmp(buf, "logdir", strlen("logdir")) == 0)
        {
            path = buf + strlen("logdir") + 1;
            path[strlen(path) - 1] = '\0';
            break;
        }
    }
    if (!path)
        goto err;

    fclose(fp);

    return (std::string)path;

err:
    if (fp)
        fclose(fp);
    fprintf(stderr, "getLogPath() error\n");
    exit(1);
}

std::string getLogFileName()
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    //std::put_time(std::localtime(&nowTime), "%Y-%m-%d %X");
    std::ostringstream nowTimeOs;
    nowTimeOs << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %X");

    return nowTimeOs.str() + LOG_FILE_SUFFIX;
}

FILE *openLogFile(const std::string &path, const std::string &file)
{
    if (path.length() == 0)
        return stderr;
    //1.创建文件夹
    DIR *dp = opendir(path.c_str());
    if (!dp)
        mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    closedir(dp);

    //2.fopen
    std::string logFile = path + file;
    FILE *fp = fopen(logFile.c_str(), "a");
    if (!fp)
    {
        fprintf(stderr, "openLogFile() error\n");
        exit(1);
    }
    return fp;
}
