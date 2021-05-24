#ifndef CHIP8_H
#define CHIP8_H

#include "chip8memory.hpp"
#include "chip8registers.hpp"
#include "chip8stack.hpp"
#include "config.hpp"
// everything goes here.  Memory, registers, pixels, etc.
struct chip8 {
  struct chip8_memory memory;
  struct chip8_stack stack;
  struct chip8_registers registers;
};

#endif
