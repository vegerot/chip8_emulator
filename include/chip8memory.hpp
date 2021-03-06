#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.hpp"
#include <cstddef>
struct chip8_memory {
  uint8_t memory[config::memory_size];
};

uint8_t chip8_memory_get(struct chip8_memory *memory, std::size_t index);
int chip8_memory_set(struct chip8_memory *memory, size_t index, uint8_t val);

uint16_t chip8_memory_get_uint16(struct chip8_memory *memory, int index);

#endif
