CC = clang
CCOPTS = -Wall -c -g -ggdb
LINKOPTS = -Wall -g -ggdb -lpthread

all: test

test: test.c disk.o
	$(CC) $(LINKOPTS) -o $@ test.c disk.o

disk.o: disk.c disk.h
	$(CC) $(CCOPTS) -o $@ disk.c

tar: totw_p6.tgz

totw_p6.tgz:
	tar -czvf totw_p6.tgz Makefile README disk.c disk.h test.c

clean:
	rm *.o
