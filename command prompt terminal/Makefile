CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors\
		 -fstack-protector-all -Wextra
PROGS = d8sh
.PHONY: all clean

all: $(PROGS)

d8sh: lexer.o executor.o d8sh.o parser.tab.o
	$(CC) -lreadline lexer.o executor.o d8sh.o parser.tab.o -o d8sh

lexer.o: parser.tab.h lexer.h 
	$(CC) $(CFLAGS) -c lexer.c

parser.tab.o: parser.tab.h
	$(CC) $(CFLAGS) -c parser.tab.c

executor.o: executor.h
	$(CC) $(CFLAGS) -c executor.c

d8sh.o: executor.h parser.tab.h lexer.h
	$(CC) $(CFLAGS) -c d8sh.c


clean:
	rm -f *.o $(PROGS) *.tmp *.out