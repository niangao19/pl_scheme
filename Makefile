CC = g++
CFLAGS	= -Wall -g -I./include
SRCDIR = src/
all: pl1.out pl.out

%.o: $(SRCDIR)%.cpp
	$(CC) -c -o $@ $<

pl1.out: $(SRCDIR)pl1.o $(SRCDIR)input_test.o
	$(CC) $^ $(CFLAGS) -o $@
pl.out: $(SRCDIR)pl3.o $(SRCDIR)scheme.o
	$(CC) $^ $(CFLAGS) -o  $@

clean:
	rm -f $(SRCDIR)*.o
	rm -f *.out
	rm -rf *.dSYM