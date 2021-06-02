#ifndef CHIP8_H
#define CHIP8_H

#include "chip8keyboard.hpp"
#include "chip8memory.hpp"
#include "chip8registers.hpp"
#include "chip8screen.hpp"
#include "chip8stack.hpp"
#include "config.hpp"
// everything goes here.  Memory, registers, pixels, etc.
struct chip8 {
  struct chip8_memory memory;
  struct chip8_stack stack;
  struct chip8_registers registers;
  struct chip8_keyboard keyboard;
  struct chip8_screen screen;
};

void chip8_init_kbd(
    struct chip8 *chip8,
    char (*WaitForKey)(char (*c8_keyboard_map)(struct chip8_keyboard *keyboard,
                                               char key),
                       struct chip8_keyboard *keyboard));
void chip8_init(struct chip8 *chip8);

void chip8_load(struct chip8 *chip8, const uint8_t *buf, size_t size);

/**
 * chip8 instruction set
 *
 * from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
 */
void chip8_exec(struct chip8 *chip8, uint16_t opcode);
#endif
