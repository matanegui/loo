#include <SDL2/SDL.h>
#include <stdbool.h>

#pragma once

typedef struct InputStateStruct {
  bool up, down, left, right;
} InputState;

typedef struct Color {
  uint8_t r, g, b, a;
} RGBAColor;

extern bool GAME_init(bool (*init_callback)());
extern bool GAME_load(bool (*load_callback)());
extern void GAME_loop(void (*update_callback)(InputState input, int delta),
                      void (*draw_callback)(SDL_Renderer* renderer));
extern void GAME_quit(void (*quit_callback)());

extern SDL_Texture* GAME_load_texture(const char* path);