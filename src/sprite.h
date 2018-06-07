#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../libs/vector.h"

#include "texture.h"

#pragma once

typedef struct {
  char* id;
  int* frames;
} Animation_State;

typedef struct {
  Animation_State* current_state;
  int current_frame;
  int speed;
  int timestamp;
  Animation_State* states;
} Animation;

typedef struct {
  Texture* texture;
  SDL_Rect* clip_rect;
  Animation* animation;
} Sprite;

extern Sprite* CreateSprite();

extern SDL_Rect* CreateClipRect(int x, int y, int w, int h);
extern void RenderSprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y);
extern void CreateSpriteAnimation(Sprite* sprite, int speed);
extern void DestroyAnimation(Animation* sprite);
extern void AddAnimationState(Sprite* sprite, char* id, int* frames);
extern void PlaySpriteAnimation(Sprite* sprite, char* state_id);
extern void UpdateSprite(Sprite* sprite, int delta);
extern void DestroySprite(Sprite* sprite);
