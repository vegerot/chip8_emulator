#include "chip8keyboard.hpp"
#include <assert.h>
void chip8_keyboard_set_map(struct chip8_keyboard *keyboard, const char *map) {
  keyboard->keyboard_map = map;
}
char chip8_keyboard_map(struct chip8_keyboard *keyboard, char key) {
  // TODO implement a constant-time solution
  // ideas:
  //  hashmap
  //  compile-time thing
  for (int i = 0; i < config::total_keys; ++i) {
    if (keyboard->keyboard_map[i] == key)
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
