#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include "SDL2/SDL.h"

#include "stack.h"
#include "chip8.h"

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 512;
const unsigned int DELAY = 2;

const uint8_t KEYBINDS[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

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



	//Emulation Main Loop
	while(true){

		emulator.cycle();

		SDL_Event event;

		bool quit = false;

		while(SDL_PollEvent(&event)){

			//Exit Out of Everything if Quit
			if(event.type == SDL_QUIT){
				quit = true;
				break;
			}

			//See if Key has been pressed
			if(event.type == SDL_KEYDOWN){

				if(event.key.keysym.sym == SDLK_ESCAPE){
					quit = true;
					break;
				}

				//For Keypad
				for(unsigned int i = 0; i < 16; i++){
					if(event.key.keysym.sym == KEYBINDS[i]){
						emulator.keypad[i] = 1;
					}
				}
			}

			//See if Key has been unpressed
			if(event.type == SDL_KEYUP){

				//For Keypad
				for(unsigned int i = 0; i < 16; i++){
					if(event.key.keysym.sym == KEYBINDS[i]){
						emulator.keypad[i] = 0;
					}
				}
			}

		}

		//Exit
		if(quit){break;}


		//Update Screen
		SDL_UpdateTexture(texture,NULL,emulator.graphics, 64 * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr,nullptr);
		SDL_RenderPresent(renderer);

		 std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));

	}



	//Free Everything
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}