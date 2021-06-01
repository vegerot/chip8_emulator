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
    assert(false);
  return memory->memory[index];
}

int chip8_memory_set(struct chip8_memory *memory, size_t index, uint8_t val) {
  // should I assert, or return -1?
  if (!chip8_memory_in_bounds(index))
    return -1;
  memory->memory[index] = val;
  return 0;
}

uint16_t chip8_memory_get_uint16(struct chip8_memory *memory, int index) {
  auto byte1 = chip8_memory_get(memory, index);
  auto byte2 = chip8_memory_get(memory, index + 1);
  return byte1 << 8 | byte2;
}
