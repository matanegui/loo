#include <SDL2/SDL.h>
#include <ctype.h>
#include <stdbool.h>

#include "game.h"

#pragma once

typedef struct {
  SDL_Texture* source_texture;
  int w, h;
} Texture;

extern Texture* CreateTexture();
extern void DestroyTexture(Texture* texture);
extern Texture* TextureFromFile(const char* path);
extern Texture* TextureFromText(const char* text, SDL_Color text_color);