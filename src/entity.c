#include "entity.h"

extern Entity* CreateEntity(int x, int y) {
  Entity* e = (Entity*)malloc(sizeof(Entity));
  e->x = x;
  e->y = y;
  e->sprite = NULL;
  e->flags = 0;
  return e;
}

extern void DrawEntity(SDL_Renderer* renderer, Entity* entity) {
  Sprite* sprite = entity->sprite;
  if (sprite != NULL) {
    RenderSprite(sprite, renderer, entity->x, entity->y);
  }
}

extern void DestroyEntity(Entity* entity) {
  DestroySprite(entity->sprite);
  free(entity);
  entity = NULL;
}