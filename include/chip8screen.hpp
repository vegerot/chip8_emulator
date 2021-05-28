#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include "config.hpp"
#include <cstddef>
struct chip8_screen {
  // does it make more sense to use bitfields?
  bool pixels[config::height][config::width];
};

void chip8_screen_set(struct chip8_screen *screen, int x, int y);

bool chip8_screen_is_set(struct chip8_screen *screen, int x, int y);

bool chip8_screen_draw_sprite(struct chip8_screen *screen, int x, int y,
                              const uint8_t sprite[], int num);

size_t get_wrapped_index(int offset, size_t size);
#endif
