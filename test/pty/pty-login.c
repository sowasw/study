/**
使用伪终端实现登录程序

执行步骤：
gcc pty-login.c -o pty-login
stty raw -echo 关闭回显
sudo ./pty-login 启动，由于关闭了回显，这行不显示
之后就可以输入用户名登录了
**/


#define _XOPEN_SOURCE 501 //for ptsname,grantpt,unlockpt

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <pty.h>//for openpty; Link with -lutil.

int main(int argc, char** argv)
{
    int mfd, sfd, pid;
    char pts_name[30];
    
    char sbuf[1024], rbuf[1024];
    int nreads, nreadr;
    
    //打开伪终端主设备
    //pfd = posix_openpt(O_RDWR);
    mfd = open("/dev/ptmx", O_RDWR);
    
    grantpt(mfd);
    unlockpt(mfd);
    
    //printf("ptsfd: %d\n", mfd);
    
    char *ptr =  ptsname(mfd);
    strncpy(pts_name, ptr, 30);
    pts_name[29] = '\0';
    
    //printf("ptsname: %s\n", pts_name);
    printf("getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
    
    //子进程打开伪终端从设备，执行登录程序，登录程序以伪终端从设备作为控制终端
    if((pid = fork()) == 0)
    {
        close(mfd);
        
        //创建会话，成为新进程组组长，之后没有控制终端
        if(setsid() < 0)
            exit(1);
        //perror("setsid: \n");
        printf("getsid: %d, getpid: %d, getppid: %d\n", getsid(getpid()), getpid(), getppid());
        
        //打开伪终端从设备为控制终端
        sfd = open(pts_name, O_RDWR);
        
        //把伪终端从设备的描述符复制到stdin，stdout，stderr
        dup2(sfd, 0);
        dup2(sfd, 1);
        dup2(sfd, 2);
        
        //exec执行login程序
        execv("/bin/login", argv);
    
        return 0;
    } 
    
    //父进程读写伪终端主设备，这里用多进程分别读写，也可以用select等实现
    else 
    { 
        //读stdin，写入伪终主设备
        if(fork() == 0)
        {
            while((nreads = read(0, sbuf, 1024)) > 0)
                write(mfd, sbuf, nreads);
            
            return 0;           
        }
        
        //读伪终端主设备，写到stdout   
        else 
        {
            while((nreadr = read(mfd, rbuf, 1024)) > 0)
                write(1, rbuf, nreadr);
        }
             
    }
    
    return 0;
}
