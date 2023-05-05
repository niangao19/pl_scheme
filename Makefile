CC = g++
CFLAGS	= -Wall -g -I./include
SRCDIR = src/
all: pl1.out pl.out



pl1.out: $(SRCDIR)pl1.cpp $(SRCDIR)input_test.cpp
	$(CC) $^ $(CFLAGS) -o  $@
pl.out: $(SRCDIR)pl3.cpp $(SRCDIR)scheme.cpp
	$(CC) $^ $(CFLAGS) -o  $@

clean:
	rm -f *.out
	rm -rf *.dSYM
