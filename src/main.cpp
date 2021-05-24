#include "SDL2/SDL.h"
#include "chip8.hpp"
#include <stdio.h>

int main(void) {

  struct chip8 chip8;
  chip8_memory_set(&chip8.memory, 0x500, 'D');
  SDL_Log("%c\n", chip8_memory_get(&chip8.memory, 0x500));
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
      if (event.type == SDL_QUIT) {
        goto out;
      }
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
