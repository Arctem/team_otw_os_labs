all: shell

shell: shell.c mystrings
	gcc -g -Wall shell.c mystrings.o -o shell

mystrings: mystrings.c mystrings.h
	gcc -g -Wall mystrings.c -c
