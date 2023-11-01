#ifndef STACK_H
#define STACK_H

#include <stdint.h>


class Stack{
private:
	uint8_t STACKSIZE = 16;

	uint16_t * stack;      //Stack Array
	uint8_t sp;			   //Stack Pointer


public:
	Stack();
	~Stack();

	void push(uint16_t pc);
	uint16_t pull();
};


#endif