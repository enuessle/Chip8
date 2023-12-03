#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "stack.h"
#include "chip8.h"

//CONSTANTS
const uint16_t START_ADDRESS = 0x200;

const uint8_t fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


//Constructor
Chip8::Chip8(){
	//Initilize Variables
	pc = START_ADDRESS;
}


//Clear the Graphics Buffer
void Chip8::_00E0(){
	memset(graphics, 0, sizeof(graphics));
}

//Set PC to value at top of Stack
void Chip8::_00EE(){
	pc = stack.pop();
}

//Jump to Address in nnn
void Chip8::_1nnn(){
	uint16_t address = opcode & 0x0FFF;

	pc = address;
}

//Jump to Address in nnn, and add it to stack
void Chip8::_2nnn(){
	uint16_t address = opcode & 0x0FFF;

	stack.push(pc);
	pc = address;
}

//If reg[x] == kk, skip next instruction
void Chip8::_3xkk(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	if(registers[x] == kk){pc+=2;}
}

//If reg[x] != kk, skip next instruction
void Chip8::_4xkk(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	if(registers[x] != kk){pc+=2;}
}

//If reg[x] == reg[y], skip next instruction
void Chip8::_5xy0(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	if(registers[x] == registers[y]){pc+=2;}
}

//Set reg[x] to kk
void Chip8::_6xkk(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	registers[x] = kk;
}

//Add: reg[x] = reg[x]+kk
void Chip8::_7xkk(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	registers[x] += kk;
}

//Set reg[x] = reg[y]
void Chip8::_8xy0(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] = registers[y];
}

//Binary Or: reg[x] = reg[x] or reg[y]
void Chip8::_8xy1(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] |= registers[y];
}

//Binary AND: reg[x] = reg[x] and reg[y]
void Chip8::_8xy2(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] &= registers[y];
}

//Binary XOR: reg[x] = reg[x] xor reg[y]
void Chip8::_8xy3(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] ^= registers[y];
}

//Add with Carry
//reg[x] + reg[y] : if result > 255, set reg[0xF] (15) to 1
void Chip8::_8xy4(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	uint16_t result = registers[x]+registers[y];

	registers[0xF] = 0;
	if(result > 255){registers[0xF] = 1;}

	registers[x] = result & 0xFF;
}

//Sub with flag
//Reg[x] - Reg[y] : if reg[x] > reg[y], set reg[0xF] to 1
void Chip8::_8xy5(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[0xF] = 0;
	if(registers[x] > registers[y]){registers[0xF] = 1;}

	registers[x] -= registers[y];
}

//Rightshift
//Reg[x] >> 1 : if smallest bit of reg[x] is 1, set reg[0xF] to 1
void Chip8::_8xy6(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	registers[0xF] = registers[x]&0x1;

	registers[x] >>= 1;
}

//Sub with flag => other way
//Reg[y] - Reg[x] : if reg[y] > reg[x], set reg[0xF] to 1
void Chip8::_8xy7(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[0xF] = 0;
	if(registers[y] > registers[x]){registers[0xF] = 1;}

	registers[x] = registers[y] - registers[x];
}

//Leftshift
//Reg[x] >> 1 : if biggest bit of reg[x] is 1, set reg[0xF] to 1
void Chip8::_8xyE(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	registers[0xF] = 0;
	if(registers[x]&0x80){registers[0xF] = 1;}

	registers[x] <<= 1;
}

//Skip next if reg[x] != reg[y]
void Chip8::_9xy0(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	if(registers[x] != registers[y]){pc+=2;}
}