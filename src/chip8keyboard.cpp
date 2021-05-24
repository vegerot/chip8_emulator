#include "chip8keyboard.hpp"
#include <assert.h>
char chip8_keyboard_map(const char *map, char key) {
  // TODO implement a constant-time solution
  // ideas:
  //  hashmap
  //  compile-time thing
  for (int i = 0; i < config::total_keys; ++i) {
    if (map[i] == key)
      return i;
  }
  return -1;
}

void chip8_keyboard_down(struct chip8_keyboard *keyboard, int key) {
  keyboard->keyboard[key] = true;
}

void chip8_keyboard_up(struct chip8_keyboard *keyboard, int key) {
  keyboard->keyboard[key] = false;
}

bool chip8_keyboard_is_down(struct chip8_keyboard *keyboard, int key) {
  return keyboard->keyboard[key];
}
