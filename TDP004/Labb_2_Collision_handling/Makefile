CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -pedantic -g

clean: 
	rm -rf *.o a.out app

aabb.o: aabb.cc
	$(CC) $(CFLAGS) -c aabb.cc

unit_test.o: unit_test.cc
	$(CC) $(CFLAGS) -c unit_test.cc

test: aabb.o unit_test.o
	$(CC) $(CFLAGS) aabb.o unit_test.o -lSDL -o app

run: test
	app testcases.txt
