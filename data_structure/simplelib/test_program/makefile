PROGRAMS = test

OBJS = test.o

#CPPLIBOPTIONS = -L../lib/
#LIBS = -lsimplelib

CC = g++

all: $(PROGRAMS) $(OBJS)

#test: test.o makefile
#	$(CC) $(CPPLIBOPTIONS) -o $(PROGRAMS) $(OBJS) $(LIBS)

#可以编译成功
test: test.o makefile ../obj/String.o
	$(CC) -o $(PROGRAMS) $(OBJS) ../obj/String.o
	
test.o: test.cpp makefile
	$(CC) -c -I../include test.cpp 
	
clean: 
	rm -f *.o $(PROGRAMS) 
