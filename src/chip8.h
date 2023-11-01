#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

class Chip8{
private:
	uint8_t registers[16];  //Registers
	uint8_t memory[4096];   //Memory Bytes
	Stack stack;			//Stack

	uint16_t index;         //Index Register
	uint16_t pc;			//Program Counter

	uint8_t timer;			//Delay Timer
	uint8_t sound_timer;	//Sound Timer

	uint8_t input;			//Input Keypad

	uint16_t opcode;		//Opcode

public:

};





#endif