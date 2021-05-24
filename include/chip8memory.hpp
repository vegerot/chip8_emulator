#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.hpp"
#include <stddef.h>
struct chip8_memory {
  unsigned char memory[config::memory_size];
};

unsigned char chip8_memory_get(struct chip8_memory *memory, size_t index);
void chip8_memory_set(struct chip8_memory *memory, size_t index,
                      unsigned char val);

#endif
