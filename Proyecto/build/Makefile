CC = g++
CFLAGS = -c -Wall -fopenmp
LFLAGS = -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

all: parlpic

parlpic:
	$(CC) $(CFLAGS) ../src/ParallelPic.cpp $(LFLAGS)

