#Kompilator:
CC=g++
#Flaggor:
CFLAGS=-std=c++11 -Wall -Wextra -pedantic -g

up1init: up1.cc
	$(CC) $(CFLAGS) up1.cc -o up1

up2init: up2.cc
	$(CC) $(CFLAGS) up2.cc -o up2

up3init: up3.cc
	$(CC) $(CFLAGS) up3.cc -o up3

up4init: up4.cc
	$(CC) $(CFLAGS) up4.cc -o up4

up1run: up1init
	up1

up2run: up2init
	up2

up3run: up3init
	up3

up4run: up4init
	up4

up1: up1run
	rm -rf up1

up2: up2run
	rm -rf up2

up3: up3run
	rm -rf up3

up4: up4run
	rm -rf up4

clean:
	rm -rf *.o *~ up1 up2 up3 up4
