PROG = mmap_test \
	shm_mmap_test

all:$(PROG)

mmap_test:mmap_test.c Makefile
	gcc mmap_test.c -o mmap_test -lpthread

shm_mmap_test:shm_mmap_test.c
	gcc shm_mmap_test.c -o shm_mmap_test -lpthread -lrt
	
clean:
	rm -f a.out *.o $(PROG)	
