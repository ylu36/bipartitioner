OBJS =  node.o driver.cpp
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

parser : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o bipartition

node.o : node.hpp node.cpp driver.cpp
	$(CC) $(CFLAGS) node.cpp

clean:
	rm -f *.o *.out bipartition output.txt
