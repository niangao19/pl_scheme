CC = g++
CFLAGS	= -Wall -g -I./include -std=c++11
SRCDIR = src/
OBJDIR = obj/
all: pl1.out pl.out

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 

pl1.out: $(OBJDIR)pl1.o $(OBJDIR)input_test.o
	$(CC) $^ $(CFLAGS) -o $@
pl.out: $(OBJDIR)pl3.o $(OBJDIR)scheme.o
	$(CC) $^ $(CFLAGS) -o  $@
test: $(OBJDIR)pl3.o
	g++ test/test_gi.cpp -lgtest -lpthread
	g++ test/input_gtest.cpp -lgtest -lpthread -o test.o
clean:
	rm -f *.out
	rm -rf *.dSYM
	rm -rf *.o
	rm -f $(OBJDIR)*.o
