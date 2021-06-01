#include "chip8.hpp"
#include "chip8keyboard.cpp"
#include "chip8memory.cpp"
#include "chip8screen.cpp"
#include "chip8stack.cpp"
#include <memory.h>

const uint8_t chip8_default_character_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // '0'
    0x20, 0x60, 0x20, 0x20, 0x70, // '1'
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // '2'
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // '3'
    0x90, 0x90, 0xF0, 0x10, 0x10, // '4'
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // '5'
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // '6'
    0xF0, 0x10, 0x20, 0x40, 0x40, // '7'
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // '8'
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // '9'
    0xF0, 0x90, 0xF0, 0x90, 0x90, // 'A'
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // 'B'
    0xF0, 0x80, 0x80, 0x80, 0xF0, // 'C'
    0xE0, 0x90, 0x90, 0x90, 0xE0, // 'D'
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // 'E'
    0xF0, 0x80, 0xF0, 0x80, 0x80  // 'F'
};

void chip8_init(struct chip8 *chip8) {
  memset(chip8, (int)NULL, sizeof(struct chip8));
  chip8->registers.sp = 0;
  chip8->registers.DT = 0;
  chip8->registers.ST = 0;
  memset(&chip8->stack.stack, (int)NULL,
         sizeof(((struct chip8 *)0)->stack.stack));
  memcpy(&chip8->memory.memory, chip8_default_character_set,
         sizeof(chip8_default_character_set));
  memset(chip8->screen.pixels, (int)NULL,
         sizeof(((struct chip8 *)0)->screen.pixels));
}

void chip8_load(struct chip8 *chip8, const uint8_t *buf, size_t size) {
  assert(size + config::program_load_address < config::memory_size);
  memcpy(&chip8->memory.memory[config::program_load_address], buf, size);
  chip8->registers.pc = config::program_load_address;
}
void chip8_exec(struct chip8 *chip8, uint16_t opcode) {
  switch (opcode) {
  case 0x00E0:
    chip8_screen_clear(&chip8->screen);
    break;
  }
}
