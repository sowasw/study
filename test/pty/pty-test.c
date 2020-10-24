
#define _XOPEN_SOURCE 501 //for ptsname,grantpt,unlockpt

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <pty.h>//for openpty; Link with -lutil.


//#define _XOPEN_SOURCE 501

int main()
{
    ///1
    int pfd;
    char pts_name[30];
    
    //pfd = posix_openpt(O_RDWR);
    pfd = open("/dev/ptmx", O_RDWR);
    
    grantpt(pfd);
    unlockpt(pfd);
    
    printf("ptsfd: %d\n", pfd);
    
    char *ptr =  ptsname(pfd);
    strncpy(pts_name, ptr, 30);
    pts_name[29] = '\0';
    
    printf("ptsname: %s\n", pts_name);
    
    //open(pts_name, O_RDWR);
    
    
    ///2
    int pfdm, pfds;
    char pts_name2[30];
    openpty(&pfdm, &pfds, pts_name2, NULL, NULL);
    printf("ptsname2: %s\n", pts_name2);
    
    //open(pts_name2, O_RDWR);
    
    //新启动一个终端执行cat  > /dev/pts/3, /dev/pts/3为pts_name2
    char buf[1024];
    int nread;
    while((nread = read(pfdm, buf, 1024)) > 0)
        write(1, buf , nread);
    
    
    
    return 0;
}
