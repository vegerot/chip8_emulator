#include "SDL.h"
#include "chip8.hpp"
#include <cstdio>
#include <stdio.h>
#include <unistd.h>

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

int main(int argc, const char *argv[]) {

  if (argc < 2) {
    printf("You must provide a file to load\n");
    return -1;
  }

  const char *filename = argv[1];
  printf("The filename to load is '%s'\n", filename);

  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Failed to open the file\n");
    return -1;
  }

  fseek(file, 0, SEEK_END);
  auto file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  uint8_t buf[config::memory_size - config::program_load_address];
  const int chunk_size = 1;
  int objects_read = fread(buf, file_size, chunk_size, file);

  if (objects_read != chunk_size) {
    std::perror("Failed to read from file");
    printf("%s\n", buf);
    return -1;
  }

  struct chip8 chip8;
  chip8_init(&chip8);
  chip8_load(&chip8, buf, file_size);

  for (int num = 0; num <= 0xF; ++num) {
    chip8_screen_draw_sprite(&chip8.screen, num * 0x5, (num / 0xD) * 5,
                             &chip8.memory.memory[0x00 + num * 5], 5);
  }

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

    if (chip8.registers.DT > 0) {
      sleep(config::frame_rate);
      --chip8.registers.DT;
      printf("delay!!!\n");
    }

    if (chip8.registers.ST > 0) {
      // Beep(8000, config::frame_rate);
      --chip8.registers.ST;
    }

    uint16_t opcode =
        chip8_memory_get_uint16(&chip8.memory, chip8.registers.pc);
    chip8_exec(&chip8, opcode);
    chip8.registers.pc += 2;
  }

out:
  printf("exiting\n");
  fclose(file);
  SDL_DestroyWindow(window);
  SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
  return 0;
}
