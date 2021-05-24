#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.hpp"
#include <_types/_uint16_t.h>
#include <_types/_uint8_t.h>
#include <stddef.h>
struct chip8_memory {
  uint16_t memory[config::memory_size];
};

uint8_t chip8_memory_get(struct chip8_memory *memory, size_t index);
void chip8_memory_set(struct chip8_memory *memory, size_t index, uint8_t val);

#endif
