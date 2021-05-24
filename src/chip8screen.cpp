#include "chip8screen.hpp"
#include <assert.h>
#include <cstdio>

static void chip8_screen_check_bounds(int x, int y) {
  assert(x >= 0 && x < config::width && y >= 0 && y < config::height);
}

void chip8_screen_set(struct chip8_screen *screen, int x, int y) {
  chip8_screen_check_bounds(x, y);
  screen->pixels[y][x] = true;
}

bool chip8_screen_is_set(struct chip8_screen *screen, int x, int y) {
  chip8_screen_check_bounds(x, y);
  // printf("screen->pixels[%d][%d] == %d\n", y, x, (int)screen->pixels[y][x]);
  return screen->pixels[y][x];
}
