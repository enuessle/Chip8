#include <stdint.h>
#include <cstdlib>

#include "stack.h"

Stack::Stack(){
	stack = (uint16_t *)calloc(STACKSIZE, sizeof(uint16_t));
	sp = 0x00;
}

Stack::~Stack(){
	free(stack);
}

void Stack::push(uint16_t pc){
	stack[sp] = pc;
	sp++;
}

uint16_t Stack::pull(){
	sp--;
	return stack[sp];
}