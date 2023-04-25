CC = g++
CFLAGS	= -Wall -g 

all: pl1.out pl.out

pl1.out: pl1.cpp
	$(CC) pl1.cpp $(CFLAGS) -o  pl1.out
pl.out: pl3.cpp
	$(CC) pl3.cpp $(CFLAGS) -o  pl

clean:
	rm -f *.out
