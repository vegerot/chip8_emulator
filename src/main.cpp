#include "SDL.h"
#include "chip8.hpp"
#include "chip8keyboard.hpp"
#include "chip8screen.hpp"
#include <stdio.h>

const char keyboard_map[config::total_keys] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

static int handleEvent(chip8 *chip8, SDL_Event *event) {
  switch (event->type) {
  case SDL_QUIT:
    return -1;
  case SDL_KEYDOWN: {
    char key = event->key.keysym.sym;
    char vkey = chip8_keyboard_map(keyboard_map, key);
    if (vkey != -1)
      chip8_keyboard_down(&chip8->keyboard, vkey);
    else
      printf("invalid key down %x\n", vkey);
    break;
  }
  case SDL_KEYUP: {
    char key = event->key.keysym.sym;
    char vkey = chip8_keyboard_map(keyboard_map, key);
    if (vkey != -1)
      chip8_keyboard_up(&chip8->keyboard, vkey);
    else
      printf("invalid key up %x\n", vkey);
    break;
  }
  }
  return 0;
}

static void renderFrame(SDL_Renderer *renderer, chip8 *chip8) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

  for (int x = 0; x < config::width; ++x) {
    for (int y = 0; y < config::height; ++y) {
      bool isSet = chip8_screen_is_set(&chip8->screen, x, y);
      // printf("isSet: %i %i %i\n", x, y, isSet);
      if (isSet) {
        SDL_Rect r;
        r.x = x * config::window_scale;
        r.y = y * config::window_scale;
        (r).w = config::window_scale;
        r.h = config::window_scale;
        SDL_RenderFillRect(renderer, &r);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

int main(void) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8_screen_set(&chip8.screen, 10, 1);

  if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) { // Initialize SDL2
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  // Create an application window with the following settings:
  auto *window = SDL_CreateWindow(
      config::title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      config::width * config::window_scale,
      config::height * config::window_scale, SDL_WINDOW_SHOWN);

  // Check that the window was successfully created
  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

  if (renderer == NULL) {
    printf("Could not create render context: %s\n", SDL_GetError());
    return 1;
  }

  for (;;) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handleEvent(&chip8, &event) == -1)
        goto out;
    }
    renderFrame(renderer, &chip8);
  }

  printf("Hello world!\n");
out:
  printf("exiting\n");
  SDL_DestroyWindow(window);
  SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
  return 0;
}
