#ifndef CHIP8_H
#define CHIP8_H

#include "chip8memory.hpp"
#include "config.hpp"
// everything goes here.  Memory, registers, pixels, etc.
struct chip8 {
  struct chip8_memory memory;
};

#endif
