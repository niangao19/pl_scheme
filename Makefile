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
test: testpl.out test.o testg.o

testpl.out:
	$(CC) pl3.cpp  $(CFLAGS) -o $(OBJDIR)pltest.o
	$(CC) $(OBJDIR)pltest.o  testpl.cpp  $(CFLAGS) -lgtest -lpthread  -o testpl.out
test.o:
	g++ test/input_gtest.cpp -lgtest -lpthread -o test.o
testg.o:
	g++ test/test_gi.cpp -lgtest -lpthread -o testg.o
clean:
	rm -f *.out
	rm -rf *.dSYM
	rm -rf *.o
	rm -f $(OBJDIR)*.o
