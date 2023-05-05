CC = g++
CFLAGS	= -Wall -g -I./include
SRCDIR = src/
all: pl1.out pl.out

pl1.out: $(SRCDIR)pl1.cpp
	$(CC) $(SRCDIR)input_test.cpp $(SRCDIR)pl1.cpp $(CFLAGS) -o  pl1.out
pl.out: $(SRCDIR)pl3.cpp
	$(CC) $(SRCDIR)scheme.cpp $(SRCDIR)pl3.cpp $(CFLAGS) -o  pl.out

clean:
	rm -f *.out
	rm -rf *.dSYM
