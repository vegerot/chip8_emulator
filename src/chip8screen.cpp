#include "chip8screen.hpp"
#include <assert.h>
#include <cstdio>
#include <memory.h>

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

bool chip8_screen_draw_sprite(struct chip8_screen *screen, int x, int y,
                              const uint8_t sprite[], int num) {
  bool pixel_collision = false;

  for (int ly = 0; ly < num; ++ly) {
    uint16_t line = sprite[ly];
    for (int lx = 0; lx < 8; ++lx) {
      if ((line & ((1 << 7) >> lx))) {
        size_t neXt = get_wrapped_index(lx + x, config::width);
        size_t nextY = get_wrapped_index(ly + y, config::height);
        pixel_collision = screen->pixels[nextY][neXt];
        screen->pixels[nextY][neXt] ^= true;
      }
    }
  }

  return pixel_collision;
}

size_t get_wrapped_index(int offset, size_t size) {
  if (offset % (int)size == 0)
    return 0;
  else if (offset >= 0) {
    return offset % (int)size;
  } else /*if (offset < 0)*/ {

    return (offset % (int)size + (int)size);
  }
}

void chip8_screen_clear(struct chip8_screen *screen) {
  memset(screen->pixels, 0, sizeof(screen->pixels));
}
