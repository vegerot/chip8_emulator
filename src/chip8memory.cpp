#include "chip8memory.hpp"
#include <assert.h>

inline static bool chip8_memory_in_bounds(size_t index) {
  // assert(index >= 0 && index < config::memory_size);
  if (index >= 0 && index < config::memory_size)
    return true;
  else
    return false;
}
uint8_t chip8_memory_get(struct chip8_memory *memory, size_t index) {
  if (!chip8_memory_in_bounds(index))
    return -1;
  return memory->memory[index];
}

int chip8_memory_set(struct chip8_memory *memory, size_t index, uint8_t val) {
  // should I assert, or return -1?
  if (!chip8_memory_in_bounds(index))
    return -1;
  memory->memory[index] = val;
  return 0;
}
