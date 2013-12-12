CC = g++
CFLAGS = -c -Wall -fopenmp
LFLAGS = -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

all: ParallelPic

ParallelPic:
	$(CC) $(CFLAGS) ../src/ParallelPic.cpp $(LFLAGS)

image:
	$(CC) $(CFLAGS) ../src/image.cpp $(LFLAGS)

