#include <sys/eventfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
	char buf[1024];
	int n;
	
	uint64_t count;
	
	memset(buf, 0, 1024);
	int efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	//int efd = eventfd(0, 0);
	
	printf("%d\n", efd);
	
	///1
	if(fork() == 0) {
		printf("child efd: %d\n", efd);
		sprintf(buf, "wwwwsss\n");
		n = write(efd, buf, strlen(buf));//只能写大于等于8字节，但只能读出8字节
		printf("child n: %d\n", n);
		exit(0);
	}
		
	sleep(1);
	printf("parent efd: %d\n", efd);
	n = read(efd, buf, 1024);
	write(1, buf, n);
	printf("parent n: %d\n", n);
	n = read(efd, buf, 1024);
	printf("parent n: %d\n", n);
	printf("parent buf: %ld\n", *((uint64_t *)buf));
	
	
	
	
	///2
	printf("\n----------------------2\n");
	
	efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC | EFD_SEMAPHORE);
	
	if(fork() == 0) {
		uint64_t wcount = 5;
		n = write(efd, &wcount, sizeof(uint64_t));
		wcount = 3;
		n = write(efd, &wcount, sizeof(uint64_t));
		printf("child n: %d\n", n);
		exit(0);
	}
		
	sleep(1);
	int readCount = 0;
	//n = read(efd, &count, sizeof(uint64_t));
	while((n = read(efd, &count, sizeof(uint64_t))) > 0) {
		printf("parent n: %d\n", n);
		printf("parent count: %ld\n", count);
		readCount++;
	}
	printf("parent readCount: %d\n", readCount);
	
	
	
	return 0;
}
