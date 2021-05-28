#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.hpp"
#include <cstdint>
#include <stddef.h>
struct chip8_memory {
  uint8_t memory[config::memory_size];
};

uint8_t chip8_memory_get(struct chip8_memory *memory, size_t index);
int chip8_memory_set(struct chip8_memory *memory, size_t index, uint8_t val);

#endif
