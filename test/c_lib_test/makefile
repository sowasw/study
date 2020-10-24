PROG = clibtest

all:$(PROG)


clibtest:main.c mylib.c mylib.h makefile
	gcc  -o $(PROG) main.c 

clean:
	rm -f $(PROG) a.out *.o
