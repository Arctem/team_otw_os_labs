all: p3

p3: p3.c buffer.o
	clang -g -Wall p3.c buffer.o -o p3 -lpthread -lrt

buffer.o: buffer.c buffer.h
	clang -g -Wall buffer.c -c

tar: totw_p3.tgz

totw_p3.tgz: p3.c buffer.c buffer.h
	tar -czvf totw_p3.tgz p3.c buffer.c buffer.h Makefile README

clean:
	rm *.o
