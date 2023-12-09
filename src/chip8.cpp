#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "stack.h"
#include "chip8.h"

//CONSTANTS
const uint16_t START_ADDRESS = 0x200;

const uint8_t VIDEO_WIDTH = 64;
const uint8_t VIDEO_HEIGHT = 32;

const uint FONTSET_ADDRESS = 80;
const uint8_t FONTSET[80] =
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


//Emulator Functions

void Chip8::loadROM(char* filename){

}


void Chip8::decode(uint16_t opcode){

	this.opcode = opcode;

	switch ( (opcode & 0xF000) >> 12 ){
		case 0x0:
			//Two for 0
			if(opcode == 0x00E0){
				_00E0();
			}
			if(opcode == 0x00EE){
				_00EE();
			}
			break;
		case 0x1:
			_1nnn();
			break;
		case 0x2:
			_2nnn();
			break;
		case 0x3:
			_3xkk();
			break;
		case 0x4:
			_4xkk();
			break;
		case 0x5:
			_5xy0();
			break;
		case 0x6:
			_6xkk();
			break;
		case 0x7:
			_7xkk();
			break;
		case 0x8:
			//Multiple for 8
			switch (opcode & 0x000F){
				case 0x0:
					_8xy0();
					break;
				case 0x1:
					_8xy1();
					break;
				case 0x2:
					_8xy2();
					break;
				case 0x3:
					_8xy3();
					break;
				case 0x4:
					_8xy4();
					break;
				case 0x5:
					_8xy5();
					break;
				case 0x6:
					_8xy6();
					break;
				case 0x7:
					_8xy7();
					break;
				case 0xE:
					_8xyE();
					break;
			}
			break;
		case 0x9:
			_9xy0();
			break;
		case 0xA:
			_Annn();
			break;
		case 0xB:
			_Bnnn();
			break;
		case 0xC:
			_Cxkk();
			break;
		case 0xD:
			_Dxyn();
			break;
		case :
			//For Starting with E and F
			switch (opcode & 0x00FF){
				case 0xA1:
					_ExA1();
					break;
				case 0x9E:
					_Ex9E();
					break;
				case 0x07:
					_Fx07();
					break;
				case 0x0A:
					_Fx0A();
					break;
				case 0x15:
					_Fx15();
					break;
				case 0x18:
					_Fx18();
					break;
				case 0x1E:
					_Fx1E();
					break;
				case 0x29:
					_Fx29();
					break;
				case 0x33:
					_Fx33();
					break;
				case 0x55:
					_Fx55();
					break;
				case 0x65:
					_Fx65();
					break;
			}
			break;
	}


}






//OPCODES

void Chip8::_skip(){}

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

//Set Index
void Chip8::_Annn(){
	uint16_t nnn = opcode & 0x0FFF;

	index = nnn;
}

//Jump to reg[0] + nnn
void Chip8::_Bnnn(){
	uint16_t nnn = opcode & 0x0FFF;

	pc = registers[0]+nnn;
}

//Set to Random byte and kk
void Chip8::_Cxkk(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	uint8_t r = rand();

	registers[x] = r & kk;
}

//Draw sprite
void Chip8::_Dxyn(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint8_t n = opcode & 0x000F;

	uint8_t xPos = registers[x] % VIDEO_WIDTH;
	uint8_t yPos = registers[y] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for(unsigned int row = 0; row < n; row++){

		if(yPos + row > VIDEO_HEIGHT){break;}

		uint8_t sprite = memory[index+row];

		for(unsigned int col = 0; col < 8; col++){

			if(xPos + col > VIDEO_WIDTH){break;}

			uint8_t new_pixel = sprite & (0x80 >> col);
			uint32_t * old_pixel = &graphics[(yPos+row) * VIDEO_WIDTH + (xPos+col)];

			if(new_pixel){

				if(*old_pixel == 0xFFFFFFFF){
					registers[0xF] = 1;
				}

				*old_pixel ^= 0xFFFFFFFF;

			}
		}
	}
}

//Skip if key with value of register[x] is pressed
void Chip8::_Ex9E(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[x];

	if(keypad[key] == key){pc+=2;}
}

//Skip if key not pressed
void Chip8::_ExA1(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[x];

	if(keypad[key] != key){pc+=2;}
}

//Set reg[x] to timer
void Chip8::_Fx07(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	registers[x] = timer;
}

//Wait for keypress
void Chip8::_Fx0A(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	uint8_t key = 0;

	while(key <= 15){
		if(keypad[key]){
			registers[x] = key;
			return;
		}
	}

	pc -= 2;
}

//Timer to reg[x]
void Chip8::_Fx15(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	timer = registers[x];
}

//Sound Timer to reg[x]
void Chip8::_Fx18(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	sound_timer = registers[x];
}

//Add to Index
void Chip8::_Fx1E(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	index += registers[x];
}

//Set I to location of sprite for digit at reg[x]
void Chip8::_Fx29(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t digit = registers[x];

	index = FONTSET_ADDRESS + (digit*5);
}

//Store Hundreds place of reg[x] at index
//Tens at index+1
//Ones at index+2
void Chip8::_Fx33(){
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[x];

	uint16_t i = index + 2;
	while(i>=index){
		memory[i] = value % 10;
		value /=10;
		i--;
	}
}

//Stores registers 0~x in memory starting at index
void Chip8::_Fx55(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	for(uint8_t i = 0; i<=x; i++){
		memory[index+i] = registers[i];
	}
}

//Set registers 0~x from memory starting at index
void Chip8::_Fx65(){
	uint8_t x = (opcode & 0x0F00) >> 8;

	for(uint8_t i = 0; i<=x; i++){
		registers[i] = memory[index+i];
	}
}