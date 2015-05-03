CC = clang
CCOPTS = -Wall -c -g -ggdb
LINKOPTS = -Wall -g -ggdb -lpthread

EXEC=test
OBJECTS=disk.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LINKOPTS) -o $@ $^

#%.o:%.c
#	$(CC) $(CCOPTS) -o $@ $^

disk.o: disk.c disk.h
	$(CC) $(CCOPTS) -o $@ disk.c

clean:
	- $(RM) $(EXEC)
	- $(RM) $(OBJECTS)
	- $(RM) *~
	- $(RM) core.*
	- $(RM) lab4.in lab4.out lab4.err
