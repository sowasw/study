/**
 *
 * 文件记录上锁
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h> 
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> /* inet(3) functions */
#include <errno.h> /* inet(3) functions */
#include <signal.h>
#include <sys/wait.h> 
#include <poll.h>
#include <limits.h>/*2.6.24以后没有open_max了*/
#include <pthread.h>
#include <signal.h> 
#include <sys/resource.h>/*void sig_int(int signo)*/
#include <fcntl.h>


#define MAXLINE 1024
//#define N_CHILDREN 1024
#define FILE_NAME "file.test"

//处理信号函数
typedef void Sigfunc(int); //简化
Sigfunc *signal(int signo, Sigfunc *func);
void sig_child(int signo);

//用于文件上锁描述符
static int lock_fd = -1;

//这里没用到
static struct flock lock_it, unlock_it;
void my_lock_init(char * pathname);
void my_lock_init2(char * pathname);
void my_lock_wait();
void my_lock_release();

//文件上锁，通用函数
int 
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

#define read_lock(fd,offset,whence,len) \
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd,offset,whence,len) \
			lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd,offset,whence,len) \
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence),(len))
#define writew_lock(fd,offset,whence,len) \
			lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence),(len))
#define un_lock(fd,offset,whence,len) \
			lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence),(len))
					
int main(int argc, char** argv) {
/*	my_lock_init("/tmp/lock.XXXXXX");//创建临时文件并初始化锁，之后unlink */
    
    signal(SIGCHLD, sig_child);
    
    lock_fd = open(FILE_NAME, O_RDWR);
    
    if(fork() == 0){//child 1
    	char buf[MAXLINE];
    	sleep(1);//让子进程2先运行
    	strcpy(buf, "\nchild1: ");//
    	
    	lseek(lock_fd, 5, SEEK_SET);//定位到文件开始+5
    	off_t start = lseek(lock_fd, 0, SEEK_CUR);//当前偏移量
    	
    	readw_lock(lock_fd, 0, SEEK_CUR, 0);//对文件当前偏移量到文件结尾加读锁
    	//readw_lock(lock_fd, 5, SEEK_SET, 0);//对文件开始偏移量+5处到文件结尾加读锁 
    	int n = read(lock_fd, buf + 9, MAXLINE - 9);//读文件到buf+9处开始
    	un_lock(lock_fd, 0, SEEK_CUR, 0);
    	//un_lock(lock_fd, 5, SEEK_SET, 0);
    	
    	write(1, buf, n + 9);
    	exit(0);
    }
    
    if(fork() == 0){//child 2
    	char buf[MAXLINE];
    	strcpy(buf, "\nchild2: ");//
    	
    	writew_lock(lock_fd, 0, SEEK_SET, 5);//对文件开始，长度为5字节的范围加写锁
    	int n = read(0, buf + 9, MAXLINE - 9);//阻塞在读标准输入上
    	un_lock(lock_fd, 0, SEEK_SET, 5);
    	
    	
    	write(1, buf, n + 9);
    	exit(0);
    }
    
    while (1)
        pause();

    return 0;
}

Sigfunc* signal(int signo, Sigfunc *func) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; //|=按位或之后赋值
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);
    return (oact.sa_handler);
}

void sig_child(int signo) {
    pid_t pid;
    int stat;

    //    pid = wait(&stat);
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)//正确应该调用waitpid,unp书上p110
        printf("child %d terminated（子进程结束）\n", pid);
    return;
}

void my_lock_init(char * pathname){
	char lockfile[1024];
	
	strncpy(lockfile, pathname, 1024);
	lock_fd = mkstemp(lockfile);//创建临时文件并立即删除
	unlink(lockfile);			//这样退出程序，临时文件也不在硬盘中
	
	lock_it.l_type = F_WRLCK;//初始化锁
	lock_it.l_whence = SEEK_SET;
	lock_it.l_start = 0;
	lock_it.l_len = 0;
	
	unlock_it.l_type = F_UNLCK;
	unlock_it.l_whence = SEEK_SET;
	unlock_it.l_start = 0;
	unlock_it.l_len = 0;
}

void my_lock_init2(char * pathname){
	char lockfile[1024];
	
	strncpy(lockfile, pathname, 1024);
	
	lock_it.l_type = F_WRLCK;//初始化锁
	lock_it.l_whence = SEEK_SET;
	lock_it.l_start = 0;
	lock_it.l_len = 0;
	
	unlock_it.l_type = F_UNLCK;
	unlock_it.l_whence = SEEK_SET;
	unlock_it.l_start = 0;
	unlock_it.l_len = 0;
}

void my_lock_wait(){
	int rc;
	//上锁
	while((rc = fcntl(lock_fd, F_SETLKW, &lock_it)) < 0){
		if(errno == EINTR)
			continue;
		else{
			fprintf(stderr, "fcntl error for my_lock_wait\n");
			exit(1);
		}	
	}
}

void my_lock_release(){
	int rc;
	//解锁
	while((rc = fcntl(lock_fd, F_SETLKW, &unlock_it)) < 0){
		if(errno == EINTR)
			continue;
		else{
			fprintf(stderr, "fcntl error for my_lock_release\n");
			exit(1);
		}
	}	
}

int 
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len){
	struct flock lock;
	
	lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset;	/* 相对l_whence的偏移量 */
	lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;		/* 0 表示直到文件结束EOF */	
	
	return (fcntl(fd, cmd, &lock));
}

