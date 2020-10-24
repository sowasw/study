PROG = avltest
OBJS = main.o \
	AVLTree.o \
	String.o \
	BSTree.o

all:$(PROG) $(OBJS)

main.o:main.cpp AVLTree.h String.h 
	g++ -c main.cpp

AVLTree.o:AVLTree.h AVLTree.cpp
	g++ -c AVLTree.cpp
	
String.o:String.h String.cpp
	g++ -c String.cpp

BSTree.o:BSTree.cpp
	g++ -c BSTree.cpp
	
avltest:main.cpp $(OBJS)
	g++ -o $(PROG) $(OBJS)
	
clean:
	rm -f *.o $(PROG)
