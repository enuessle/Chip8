#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>

#include "SDL2/SDL.h"

#include "stack.h"
#include "chip8.h"

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 512;
const unsigned int DELAY = 3;

int WinMain(){
	
	//Check Usage
	if(__argc != 2){
		perror("Invalid Usage: main <ROM_FILE>\n");
		exit(EXIT_FAILURE);
	}


	//Create the Chip8 Emulator
	Chip8 emulator = Chip8();

	//Load Rom
	char* rom = __argv[1];
	emulator.loadROM(rom);

	//Start SDL Stuff
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		perror("SDL could not Initilize\n");
		exit(EXIT_FAILURE);
	}

	SDL_Window* window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED, WIDTH,
											HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
											SDL_TEXTUREACCESS_STREAMING, 64, 32);





	//Free Everything
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}