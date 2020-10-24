PROGRAMS = lib/libsimplelib.so

OBJS = obj/String.o

INCOPTIONS = -Iinclude

CC = g++

all: $(PROGRAMS) $(OBJS)

lib/libsimplelib.so: $(OBJS)
	$(CC) -shared -o $(PROGRAMS) $(OBJS) 
	
obj/String.o: src/String.cpp include/String.h
	$(CC) -fPIC -c $(INCOPTIONS) -o obj/String.o src/String.cpp 
	
clean: 
	rm -f $(OBJS) $(PROGRAMS) include/*.gch
