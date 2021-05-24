#include "SDL2/SDL.h"
#include "chip8.hpp"
#include "chip8keyboard.hpp"
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

int main(void) {

  struct chip8 chip8;

  chip8_keyboard_down(&chip8.keyboard, 0x0f);
  chip8_keyboard_up(&chip8.keyboard, 0x0f);
  printf("%i\n", chip8_keyboard_is_down(&chip8.keyboard, 0x0f));

  chip8.registers.sp = 0;

  chip8_stack_push(&chip8, 0xff);
  chip8_stack_push(&chip8, 0xaa);

  printf("0x%x\n", chip8_stack_pop(&chip8));
  printf("0x%x\n", chip8_stack_pop(&chip8));
  printf("0x%x\n", chip8_stack_pop(&chip8));
  // printf("0x%x\n", chip8_stack_pop(&chip8));

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

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handleEvent(&chip8, &event) == -1)
        goto out;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_Rect rc;
    SDL_Rect *r = &rc;
    r->x = 0;
    r->y = 0;
    (*r).w = 40;
    r->h = 40;
    SDL_RenderFillRect(renderer, r);
    SDL_RenderPresent(renderer);
  }

  printf("Hello world!\n");
out:
  SDL_DestroyWindow(window);
  SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
  return 0;
}
