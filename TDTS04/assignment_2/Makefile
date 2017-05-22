CC=g++
CFLAGS=-std=c++11
PORT=25565

all: proxyserver

proxyserver: proxyserver.o
	$(CC) $(CFLAGS) proxyserver.o -o proxyserver

proxyserver.o: proxyserver.cc
	$(CC) $(CFLAGS) -c proxyserver.cc

run: proxyserver
	./proxyserver $(PORT)

clean:
	rm -rf proxyserver.o proxyserver
