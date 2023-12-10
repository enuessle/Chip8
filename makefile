# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g -static-libgcc -static-libstdc++

# Compile

main : main.o chip8.o stack.o
	$(CC) $(CFLAGS) -o main.exe main.o chip8.o stack.o -lSDL2 -lSDL2main

main.o :
	$(CC) $(CFLAGS) -c src/main.cpp

chip8.o : stack.o
	$(CC) $(CFLAGS) -c src/chip8.cpp

stack.o :
	$(CC) $(CFLAGS) -c src/stack.cpp

clean :
	rm main.o chip8.o stack.o

winclean:
	del main.o chip8.o stack.o