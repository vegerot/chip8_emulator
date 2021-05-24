#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include "config.hpp"
struct chip8_screen {
  // does it make more sense to use bitfields?
  bool pixels[config::height][config::width];
};

void chip8_screen_set(struct chip8_screen *screen, int x, int y);

bool chip8_screen_is_set(struct chip8_screen *screen, int x, int y);

#endif
