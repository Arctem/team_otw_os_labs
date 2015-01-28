all: myshell

myshell: myshell.c mystrings
	gcc -g -Wall myshell.c mystrings.o -o myshell

mystrings: mystrings.c mystrings.h
	gcc -g -Wall mystrings.c -c
