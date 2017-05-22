CC=g++
CFLAGS=-Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++0x -pedantic -Werror -g

all: main.o battery.o component.o connection.o resistor.o capacitor.o
	$(CC) $(CFLAGS) main.o battery.o component.o connection.o resistor.o capacitor.o -o main

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

battery.o: battery.cc battery.h
	$(CC) $(CFLAGS) -c battery.cc

component.o: component.cc component.h
	$(CC) $(CFLAGS) -c component.cc

connection.o: connection.cc connection.h
	$(CC) $(CFLAGS) -c connection.cc

resistor.o: resistor.cc resistor.h
	$(CC) $(CFLAGS) -c resistor.cc

capacitor.o: capacitor.cc capacitor.h
	$(CC) $(CFLAGS) -c capacitor.cc

clean:
	rm -rf *.o *~ main
