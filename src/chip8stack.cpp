#include "chip8stack.hpp"
#include "chip8.hpp"
#include <assert.h>

void chip8_stack_push(struct chip8 *chip8, uint16_t val) {
  chip8->stack.stack[chip8->registers.sp++] = val;
  assert(chip8->registers.sp >= 0 &&
         chip8->registers.sp <= config::stack_depth);
  return;
}

uint16_t chip8_stack_pop(struct chip8 *chip8) {
  // about to decrement `sp`, so `sp` must be at least one
  assert(chip8->registers.sp >= 1 && chip8->registers.sp < config::stack_depth);
  return chip8->stack.stack[--chip8->registers.sp];
}
