#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.hpp"
struct chip8_registers {
  unsigned char V[config::gp_register_count];

  unsigned short I;

  unsigned char delay_timer;
  unsigned char sound_timer;

  unsigned short pc;
  unsigned char sp;
};

#endif
