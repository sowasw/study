PROGS = fifotest \
  # pipetest

CC = gcc

all:$(PROGS)

fifotest:fifo.c
	$(CC) fifo.c -o fifotest
	
#pipetest:pipe.c
#	$(CC) pipe.c -o pipetest
	
clean:
	rm -f $(PROGS) a.out *.o
