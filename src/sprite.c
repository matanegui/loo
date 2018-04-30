#include "sprite.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#pragma once

extern Sprite* create_sprite() {
  Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
  sprite->source = NULL;
  sprite->rect = NULL;
};

extern void set_sprite_source(Sprite* sprite, char* path) {
  sprite->source = GAME_load_texture(path);
}

extern void set_sprite_rect(Sprite* sprite, int x, int y, int w, int h) {
  sprite->rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
  sprite->rect->x = 0;
  sprite->rect->y = 0;
  sprite->rect->w = w;
  sprite->rect->h = h;
}

extern void render_sprite(Sprite* sprite, SDL_Renderer* renderer, int x,
                          int y) {
  SDL_Rect render_rect = (SDL_Rect){x, y, sprite->rect->w, sprite->rect->h};
  SDL_RenderCopy(renderer, sprite->source, sprite->rect, &render_rect);
}

extern void destroy_sprite(Sprite* sprite) {
  free(sprite->rect);
  sprite->rect = NULL;
  SDL_DestroyTexture(sprite->source);
  sprite->source = NULL;
  free(sprite);
  sprite = NULL;
}