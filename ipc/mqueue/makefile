PROGS = mqueue_creat \
	mqueue_test

CC = gcc

all:$(PROGS)

mqueue_test:mqueue_test.c
	$(CC) mqueue_test.c -o mqueue_test -lrt
	
mqueue_creat:mqueue_creat.c
	$(CC) mqueue_creat.c -o mqueue_creat -lrt
	

	
clean:
	rm -f $(PROGS) a.out *.o
