CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -pedantic -g

all: main.o test1.h test2.h test2.o
	$(CC) $(CFLAGS) main.o test2.o -o app

main.o: main.cc
	$(CC) $(CFLAG) -c main.cc

test2.o: test2.cc
	$(CC) $(CFLAGS) -c test2.cc

clean:
	rm -rf *.o app *~
