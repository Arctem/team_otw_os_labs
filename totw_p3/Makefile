all: p3

p3: p3.c
	clang -g -Wall p3.c -o p3 -lpthread

tar: totw_p3.tgz

totw_p3.tgz: p3.c
	tar -czvf totw_p2.tgz p3.c Makefile README

clean:
	rm *~
	rm *.o
