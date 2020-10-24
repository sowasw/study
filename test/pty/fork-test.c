#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>


int main()
{
    int pid;
    char buf[1024];
    
    char hostname[16];
    gethostname(hostname, 16);
    printf("%s@%s\n", getlogin(), hostname);
    
    if((pid = fork()) == 0)
    {
        while(read(0, buf, 1024) > 0) 
            fprintf(stderr, "pid: %d, ppid: %d, uid: %d, gid: %d, pgid: %d, sid: %d\n", 
                            getpid(), getppid(), getuid(), getgid(), getpgrp(), getsid(getpid()));
        //sleep(20);
        
        fprintf(stderr, "exit, pid: %d, ppid: %d, errno: %s\n", getpid(), getppid(), strerror(errno));
        perror("");
        return 0;
    }
    
    while(read(0, buf, 1024) > 0) 
        fprintf(stderr, "pid: %d, ppid: %d, uid: %d, gid: %d, pgid: %d, sid: %d\n", 
                            getpid(), getppid(), getuid(), getgid(), getpgrp(), getsid(getpid()));
    
    fprintf(stderr, "exit, pid: %d, ppid: %d, errno: %s\n", getpid(), getppid(), strerror(errno));
    perror("");
    //sleep(10);
    exit(0);
}
