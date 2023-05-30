CC = g++
CFLAGS	= -Wall  -I./include -std=c++11
SRCDIR = src/
OBJDIR = obj/
all: pl1.out pl.out

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 

pl1.out: $(OBJDIR)pl1.o $(OBJDIR)input_test.o
	$(CC) $^ $(CFLAGS) -o $@
pl.out: $(OBJDIR)pl3.o $(OBJDIR)scheme.o
	$(CC) $^ $(CFLAGS) -o  $@

test:  test.o testg.o
	$(CC) $(SRCDIR)pl3.cpp   test/testpl.cpp  -I./include -fprofile-arcs -ftest-coverage -std=c++11 -lgtest -lpthread  -o testpl.out
test.o:
	g++ test/input_gtest.cpp -lgtest -lpthread -o test.o
testg.o:
	g++ test/test_gi.cpp  -lgtest -lpthread -o testg.o
clean:
	rm -f *.out
	rm -rf *.dSYM
	rm -rf *.o
	rm -f $(OBJDIR)*.o
