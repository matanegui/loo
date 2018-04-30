#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#pragma once

typedef struct SpriteStruct {
  SDL_Texture* source;
  SDL_Rect* rect;
} Sprite;

extern Sprite* create_sprite();

extern void set_sprite_source(Sprite* sprite, char* path);
extern void set_sprite_rect(Sprite* sprite, int x, int y, int w, int h);
extern void render_sprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y);
extern void destroy_sprite(Sprite* sprite);
