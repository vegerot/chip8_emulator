#include "chip8.hpp"
#include <memory.h>

void chip8_init(struct chip8 *chip8) {
  memset(chip8, (int)NULL, sizeof(struct chip8));
}
