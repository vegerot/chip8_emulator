#include "chip8memory.hpp"
#include <assert.h>

inline void chip8_memory_in_bounds(size_t index) {
  assert(index >= 0 && index < config::memory_size);
}
unsigned char chip8_memory_get(struct chip8_memory *memory, size_t index) {
  chip8_memory_in_bounds(index);
  return memory->memory[index];
}
void chip8_memory_set(struct chip8_memory *memory, size_t index,
                      unsigned char val) {
  chip8_memory_in_bounds(index);
  memory->memory[index] = val;
}
