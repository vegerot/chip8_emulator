#include "chip8.hpp"
#include "chip8keyboard.cpp"
#include "chip8keyboard.hpp"
#include "chip8memory.cpp"
#include "chip8memory.hpp"
#include "chip8screen.cpp"
#include "chip8screen.hpp"
#include "chip8stack.cpp"
#include <ctime>
#include <memory.h>
#include <stdlib.h>

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

void chip8_init_kbd(
    struct chip8 *chip8,
    char (*WaitForKey)(char (*c8_keyboard_map)(struct chip8_keyboard *keyboard,
                                               char key),
                       struct chip8_keyboard *keyboard)) {
  chip8->keyboard.WaitForKey = WaitForKey;
}

void chip8_init(struct chip8 *chip8) {
  memset(chip8, (int)NULL, sizeof(struct chip8));
  // these may not be necessary
  /*
  chip8->registers.pc = 0;
  chip8->registers.sp = 0;
  chip8->registers.DT = 0;
  chip8->registers.ST = 0;
  */
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

/**
 * switch when opcode = 0x8???
 */
static void chip8_exec_extended_8(struct chip8 *chip8, uint16_t opcode) {
  assert((opcode & 0xf000) >> 12 == 0x8);

  uint8_t x = (opcode >> 8) & 0x000f;

  uint8_t y = ((opcode & 0x00f0) >> 4);

  uint8_t n = opcode & 0x000f;

  chip8_registers *const registers = &chip8->registers;

  switch (n) {
  // LD Vx, Vy
  case 0x0:
    registers->V[x] = registers->V[y];
    break;
  // XOR
  case 0x1:
    registers->V[x] |= registers->V[y];
    break;
  // AND
  case 0x2:
    registers->V[x] &= registers->V[y];
    break;
  // XOR
  case 0x3:
    registers->V[x] ^= registers->V[y];
    break;
  // ADD
  case 0x4: {
    int tmp = (int)registers->V[x] + (int)registers->V[y];
    registers->V[0x0F] = tmp > 0xff;
    registers->V[x] = (uint8_t)tmp & 0xff;
    break;
  }
  // SUB
  case 0x5: {
    registers->V[0x0F] = registers->V[x] > registers->V[y];
    int tmp = (int)registers->V[x] - (int)registers->V[y];
    registers->V[x] = (uint8_t)tmp & 0xff;
    break;
  }
  // SHR
  case 0x6:
    registers->V[0xf] = registers->V[x] & 0b1;
    registers->V[x] /= 2;
    break;
  // SUBN
  case 0x7: {
    registers->V[0x0F] = registers->V[y] > registers->V[x];
    int tmp = (int)registers->V[y] - (int)registers->V[x];
    registers->V[x] = (uint8_t)tmp & 0xff;
    break;
  }
  case 0xE:
    registers->V[0xF] = (registers->V[x] & (1 << 7)) >> 7;
    // played around in cling repo, and this seemed to work
    registers->V[x] <<= 1;
    break;
  default:
    printf("unknown 8 opcode: %#x\n", opcode);
  }
}

static char chip8_wait_for_keypress(struct chip8 *chip8) {
  return chip8->keyboard.WaitForKey(chip8_keyboard_map, &chip8->keyboard);
}

static void chip8_exec_extended_F(struct chip8 *chip8, uint16_t opcode) {
  assert((opcode & 0xf000) >> 12 == 0xF);

  uint8_t x = (opcode >> 8) & 0x000f;

  uint8_t kk = opcode & 0x00ff;

  chip8_registers *const registers = &chip8->registers;

  switch (kk) {
  // LD
  case 0x07:
    registers->V[x] = registers->DT;
    break;
  // LD
  // Q: How to test?
  case 0x0A:
    registers->V[x] = chip8_wait_for_keypress(chip8);
    break;
  // LD
  case 0x15:
    registers->DT = registers->V[x];
    break;
  // LD
  case 0x18:
    registers->ST = registers->V[x];
    break;
  // ADD
  case 0x1E:
    registers->I += registers->V[x];
    break;
  // LD
  case 0x29:
    registers->I = registers->V[x] * config::sprite_height;
    break;
  // LD
  case 0x33: {
    auto hundreds = registers->V[x] / 100;
    auto tens = (registers->V[x] % 100) / 10;
    auto ones = (registers->V[x] % 10) / 1;

    int digits[3] = {hundreds, tens, ones};

    for (size_t i = 0; i < 3; ++i) {
      chip8_memory_set(&chip8->memory, registers->I + i, digits[i]);
    }
  } break;
  // LD
  case 0x55:
    for (int i = 0; i <= x; ++i)
      chip8_memory_set(&chip8->memory, registers->I + i, registers->V[i]);
    break;
  // LD
  case 0x65:
    for (int i = 0; i <= x; ++i)
      registers->V[i] = chip8_memory_get(&chip8->memory, registers->I + i);
    break;
  default:
    printf("unknown F opcode: %#x\n", opcode);
  }
}
void chip8_exec(struct chip8 *chip8, uint16_t opcode) {
  /**
   * nibble - the first nibble of the instruction
   */
  uint8_t H = (opcode & 0xf000) >> 12;

  /**
   * 12-bit value, the lowest 12 bits of the instruction
   */
  uint16_t nnn = opcode & 0x0fff;

  /**
   * 4-bit value - the lower 4 bits of the high byte of the instruction
   */
  uint8_t x = (opcode >> 8) & 0x000f;

  /**
   * 4-bit value - the upper 4 bits of the low byte of the instruction
   */
  uint8_t y = ((opcode & 0x00f0) >> 4);

  /**
   * 4-bit value - the lowest 4 bits
   * height of sprite
   */
  uint8_t n = opcode & 0x000f;

  /**
   * 8-bit value - the lowest 8 bits of the instruction
   */
  uint8_t kk = opcode & 0x00ff;

  chip8_registers *const registers = &chip8->registers;
  // switch on all 16 bits
  switch (H) {
  case 0x0:
    // printf("opcode: 0x%x\n", opcode);
    // assert(y == 0xE);
    switch (n) {
      /**
       * CLS: Clear The Display
       *
       * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0
       */
    case 0x0:
      chip8_screen_clear(&chip8->screen);
      break;

      /**
       * RET: return from a subroutine
       */
    case 0xE:
      chip8->registers.pc = chip8_stack_pop(chip8);
      break;
    }
    break;

    // JP addr, 1nnn Jump to location `nnn`
  case 0x1:
    registers->pc = nnn;
    break;
    // CALL addr: Call subroutine at `nnn`
  case 0x2:
    chip8_stack_push(chip8, registers->pc);
    registers->pc = nnn;
    break;

    // SE Vx, byte: Skip next instruction if `Vx` == `kk`.
  case 0x3:
    if (registers->V[x] == kk)
      registers->pc += 2;
    break;

    // SNE Vx, byte: Skip next instruction if `Vx` != `kk`.
  case 0x4:
    if (registers->V[x] != kk)
      registers->pc += 2;
    break;

    // SE Vx, Vy: Skip next instruction if `Vx == Vy`
  case 0x5:
    assert(n == 0x0);
    if (registers->V[x] == registers->V[y])
      registers->pc += 2;
    break;

    // LD Vx, byte.  Put `kk` into register `Vx`
  case 0x6:
    registers->V[x] = kk;
    break;

    // ADD Vx, kk
  case 0x7:
    registers->V[x] += kk;
    break;

  case 0x8:
    chip8_exec_extended_8(chip8, opcode);
    break;

    // SNE
  case 0x9:
    assert(n == 0);
    if (registers->V[x] != registers->V[y]) {
      registers->pc += 2;
    }
    break;

    // LD
  case 0xA:
    registers->I = nnn;
    break;

    // JP
  case 0xB:
    registers->pc = registers->V[0] + nnn;
    break;

    // RND
  case 0xC:
    srand(clock());
    registers->V[x] = (rand() % 0xFF) & kk;
    break;
    // DRW
  case 0xD: {
    // n = height of sprite
    const auto sprite = &chip8->memory.memory[registers->I];
    registers->V[0xF] = chip8_screen_draw_sprite(
        &chip8->screen, registers->V[x], registers->V[y], sprite, n);
    break;
  }
    // kbd ops
  case 0xE:
    switch (kk) {
    // SKP
    case 0x9e:
      if (chip8_keyboard_is_down(&chip8->keyboard, registers->V[x]))
        registers->pc += 2;
      break;

    // SNKP
    case 0xA1:
      if (!chip8_keyboard_is_down(&chip8->keyboard, registers->V[x]))
        registers->pc += 2;
      break;
    }
    break;
  case 0xF:
    chip8_exec_extended_F(chip8, opcode);
    break;
  default:
    printf("unknown opcode: %#x\n", opcode);
  }
}
