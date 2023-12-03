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

	uint16_t opcode;		//Current Opcode

	uint32_t graphics[64*32]; //Graphics Buffer

public:

	Chip8();

	//Emulator Functions
	void loadROM(char* filename);

	//Opcode Functions
	void _1nnn();
	void _2nnn();
	void _3xkk();
	void _4xkk();
	void _5xy0();
	void _6xkk();
	void _7xkk();
	void _9xy0();
	void _Annn();
	void _Bnnn();
	void _Cxkk();
	void _Dxyn();

	void _8xy0();
	void _8xy1();
	void _8xy2();
	void _8xy3();
	void _8xy4();
	void _8xy5();
	void _8xy6();
	void _8xy7();
	void _8xyE();

	void _00E0();
	void _00EE();

	void _ExA1();
	void _Ex9E();

	void _Fx07();
	void _Fx0A();
	void _Fx15();
	void _Fx18();
	void _Fx1E();
	void _Fx29();
	void _Fx33();
	void _Fx55();
	void _Fx65();

};





#endif