# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g

# Compile

main : main.o chip8.o stack.o
	$(CC) $(CFLAGS) -o main.out main.o chip8.o stack.o

main.o :
	$(CC) $(CFLAGS) -c src/main.cpp

chip8.o : stack.o
	$(CC) $(CFLAGS) -c src/chip8.cpp

stack.o :
	$(CC) $(CFLAGS) -c src/stack.cpp

clean :
	rm main.o chip8.o stack.o