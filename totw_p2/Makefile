all: shell

shell: shell.c
	gcc -g -Wall shell.c -o shell -lreadline

tar: totw_p2.tgz

totw_p2.tgz: shell.c
	tar -czvf totw_p2.tgz shell.c Makefile README test_script

clean:
	rm *~
	rm *.o
