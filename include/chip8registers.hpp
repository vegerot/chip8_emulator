#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.hpp"
#include <cstdint>
struct chip8_registers {
  uint8_t V[config::gp_register_count];

  uint16_t I;

  uint8_t DT;
  uint8_t ST;

  uint16_t pc;
  uint8_t sp;
};

#endif
