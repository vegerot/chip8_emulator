#ifndef CHIP8STACK_H
#define CHIP8STACK_H
#include "config.hpp"
#include <_types/_uint16_t.h>

struct chip8;
struct chip8_stack {
  uint16_t stack[config::stack_depth];
};

void chip8_stack_push(struct chip8 *chip8, uint16_t val);
uint16_t chip8_stack_pop(struct chip8 *chip8);

#endif
