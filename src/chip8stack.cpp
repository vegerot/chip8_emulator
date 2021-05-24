#include "chip8stack.hpp"
#include "chip8.hpp"
#include <assert.h>

inline void chip8_stack_in_bounds(struct chip8 *chip8) {
  assert(chip8->registers.sp < config::stack_depth);
}

void chip8_stack_push(struct chip8 *chip8, uint16_t val) {
  chip8->stack.stack[chip8->registers.sp++] = val;
  chip8_stack_in_bounds(chip8);
  return;
}

uint16_t chip8_stack_pop(struct chip8 *chip8) {
  chip8_stack_in_bounds(chip8);
  return chip8->stack.stack[--chip8->registers.sp];
}
