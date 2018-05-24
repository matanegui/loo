#include <SDL2/SDL.h>

#include "sprite.h"

#pragma once

enum {
  FLAG_VISIBLE = 1 << 0,
  FLAG_SOLID = 1 << 1,
};

typedef struct {
  int x, y;
  unsigned long flags;
  Sprite* sprite;
} Entity;

extern Entity* CreateEntity(int x, int y);
extern void DrawEntity(SDL_Renderer* renderer, Entity* entity);
extern void DestroyEntity(Entity* entity);