# Chip8
A full working emulator for the CHIP-8 interpreted programming language, coded in c++.

Uses SDL2 to both render the graphics, and receive user input.


# Emulator Specifics

The emulator will try to run most opcodes as fast as it can, with no delay limit, however it makes sure every draw function only happens at most 60 frames a second. This means for games like Tetris that run on CHIP-8, it will play smooth and actions like moving or rotating a block has minimal inpact compared to the block just falling.