PROGS = socketpairtest \
	pipetest

CC = gcc

all:$(PROGS)

socketpairtest:socketpair.c
	$(CC) socketpair.c -o socketpairtest
	
pipetest:pipe.c
	$(CC) pipe.c -o pipetest
	
clean:
	rm -f $(PROGS) a.out *.o
