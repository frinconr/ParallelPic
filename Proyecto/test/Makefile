# This makefile is intented to compile every test made in the development of this library.

CC = g++
CFLAGS = -fopenmp
LFLAGS = ../build/ParallelPic.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
EFLAGS = -o

# All the targets
all: 

hello: hello.cpp
	$(CC) $(CFLAGS) hello.cpp $(EFLAGS) hello.exe 

suma:	suma.cpp
	$(CC) $(CFLAGS) suma.cpp $(EFLAGS) suma.exe $(LFLAGS)

sumsec:	sumsec.cpp
	$(CC) $(CFLAGS) sumsec.cpp $(EFLAGS) sumsec.exe $(LFLAGS)

hellomp: hellomp.cpp
	$(CC) $(CFLAGS) hellomp.cpp $(EFLAGS) hello.exe 

sumasch: sumasch.cpp
	$(CC) $(CFLAGS) sumasch.cpp $(EFLAGS) sumasch.exe $(LFLAGS)

coorrel_par: coorrel_par.cpp
	$(CC) $(CFLAGS) coorrel_par.cpp $(EFLAGS) coorrel_par.exe $(LFLAGS)
coorrel: correl.cpp
	$(CC) $(CFLAGS) correl.cpp $(EFLAGS) coorrel.exe $(LFLAGS)
	
HSV: HSV.cpp
	$(CC) $(CFLAGS) HSV.cpp $(EFLAGS) HSV.exe $(LFLAGS)
	
average_omp: average_omp.cpp
	$(CC) $(CFLAGS) average_omp.cpp $(EFLAGS) average_omp.exe $(LFLAGS)
