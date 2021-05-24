#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.hpp"
#include <_types/_uint16_t.h>
#include <_types/_uint8_t.h>
struct chip8_registers {
  uint8_t V[config::gp_register_count];

  uint16_t I;

  uint8_t delay_timer;
  uint8_t sound_timer;

  uint16_t pc;
  uint8_t sp;
};

#endif
