#
# Makefile för bruteforcelösning
#

CCC	= g++ -std=c++11 
LFLAGS	= -L/usr/lib/x86_64-linux-gnu -lSDL -lm

all: brute.cc Point.o
	$(CCC) -o brute brute.cc Point.o $(LFLAGS)

Point.o: Point.cc Point.h
	$(CCC) -c Point.cc

clean:
	@ \rm -rf *.o core

zap: clean
	@ \rm -f fast *~

fast: fast.cc Point.o
	$(CCC) -o fast fast.cc Point.o $(LFLAGS)

fasto: fast.cc Point.o
	$(CCC) -o fast fast.cc Point.o $(LFLAGS) -O3

fast_run: fast
	./fast < mystery10089.txt

fasto_run: fasto
	./fast < mystery10089.txt
