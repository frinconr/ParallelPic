# This makefile is intented to compile every test made in the development of this library.

CC = g++
CFLAGS = -fopenmp
LFLAGS = ../build/ParallelPic.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
EFLAGS = -o

# All the targets
all: hello suma

hello: 
	$(CC) $(CFLAGS) hello.cpp $(EFLAGS) hello.exe 

suma:
	$(CC) $(CFLAGS) suma.cpp $(EFLAGS) suma.exe $(LFLAGS)

sumsec:
	$(CC) $(CFLAGS) sumsec.cpp $(EFLAGS) sumsec.exe $(LFLAGS)
