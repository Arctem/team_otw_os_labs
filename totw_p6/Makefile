CC = clang
CCOPTS = -Wall -c -g -ggdb
LINKOPTS = -Wall -g -ggdb -lpthread

all: test

test: test.c disk.o fs.o
	$(CC) $(LINKOPTS) -o $@ test.c disk.o fs.o

disk.o: disk.c disk.h
	$(CC) $(CCOPTS) -o $@ disk.c

fs.o: fs.c fs.h
	$(CC) $(CCOPTS) -o $@ fs.c

tar: totw_p6.tgz

totw_p6.tgz:
	tar -czvf totw_p6.tgz Makefile README disk.c disk.h test.c fs.c fs.h

clean:
	rm *.o
