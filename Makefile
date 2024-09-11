CC=g++
CFLAGS=-std=c++11 -Wall

all: lcs floyd

lcs: lcs.cpp
	$(CC) $(CFLAGS) -o lcs lcs.cpp

floyd: floyd.cpp
	$(CC) $(CFLAGS) -o floyd floyd.cpp

clean:
	rm -f lcs floyd

