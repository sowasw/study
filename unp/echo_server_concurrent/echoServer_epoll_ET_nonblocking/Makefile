PROG = epoll_ET_echo_server

CC = gcc

all:$(PROG)

epoll_ET_echo_server:epoll_ET_echo_server_main.c wxl_head.h wxl_tcp.h wxl_tcp.c
	$(CC) -o epoll_ET_echo_server epoll_ET_echo_server_main.c wxl_tcp.c
	
clean:
	rm -f $(PROG) *.o 
