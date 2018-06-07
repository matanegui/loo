#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#pragma once

typedef struct InputStateStruct {
  bool up, down, left, right;
} InputState;

extern bool GAME_Init(bool (*init_callback)());
extern bool GAME_Load(bool (*load_callback)());
extern void GAME_Loop(void (*update_callback)(InputState* input, int delta),
                      void (*draw_callback)(SDL_Renderer* renderer));
extern void GAME_Quit(void (*quit_callback)());

extern SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
extern SDL_Surface* CreateSurfaceFromPNG(const char* path);
extern SDL_Surface* CreateSurfaceFromText(const char* text,
                                          SDL_Color text_color);