CC = g++
CFLAGS	= -Wall -g -I./include
SRCDIR = src/
OBJDIR = obj/
OBJS = $(OBJDIR)pl1.o $(OBJDIR)input_test.o $(OBJDIR)pl3.o $(OBJDIR)scheme.o
all: pl1.out pl.out

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 

pl1.out: $(OBJDIR)pl1.o $(OBJDIR)input_test.o
	$(CC) $^ $(CFLAGS) -o $@
pl.out: $(OBJDIR)pl3.o $(OBJDIR)scheme.o
	$(CC) $^ $(CFLAGS) -o  $@

clean:
	rm -f *.out
	rm -rf *.dSYM
	rm -f $(OBJDIR)*.o
