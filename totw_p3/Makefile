all: p3

shell: p3.c
	gcc -g -Wall p3.c -o p3

tar: totw_p3.tgz

totw_p2.tgz: shell.c
	tar -czvf totw_p2.tgz p3.c Makefile README

clean:
	rm *~
	rm *.o
