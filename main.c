#include "src/game.h"
#include "src/sprite.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Test
typedef struct EntityStruct {
  int x, y;
  SDL_Texture* sprite_texture;
  SDL_Rect* sprite_rect;
} Entity;

Entity* create_entity(int x, int y) {
  Entity* e = (Entity*)malloc(sizeof(Entity));
  e->x = x;
  e->y = y;
  e->sprite_texture = NULL;
  e->sprite_rect = NULL;
  return e;
}

void destroy_entity(Entity* entity) {
  free(entity->sprite_rect);
  entity->sprite_rect = NULL;
  SDL_DestroyTexture(entity->sprite_texture);
  entity->sprite_texture = NULL;
  free(entity);
  entity = NULL;
}
// Test entity
Entity* thing;

// Setup
bool init() { return true; }

bool load() {
  thing = create_entity(0, 0);
  thing->sprite_texture = GAME_load_texture("assets/boi-24.png");
  thing->sprite_rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
  thing->sprite_rect->x = 0;
  thing->sprite_rect->y = 0;
  thing->sprite_rect->w = 24;
  thing->sprite_rect->h = 24;
  return true;
};

// Main loop
void update(InputState input, int delta) {
  const float speed = 0.15f;
  const int move_distance = speed * delta;
  if (input.up) {
    thing->y = thing->y - move_distance;
  }
  if (input.down) {
    thing->y = thing->y + move_distance;
  }
  if (input.left) {
    thing->x = thing->x - move_distance;
  }
  if (input.right) {
    thing->x = thing->x + move_distance;
  }
}

void draw(SDL_Renderer* renderer) {
  // draw thing
  SDL_Rect render_rect = (SDL_Rect){thing->x, thing->y, thing->sprite_rect->w,
                                    thing->sprite_rect->h};
  SDL_RenderCopy(renderer, thing->sprite_texture, thing->sprite_rect,
                 &render_rect);
}

// Close and clean up
void quit() { destroy_entity(thing); }

int main(int argc, char* argv[]) {
  const char* fail_message = "Something failed. Killing.\n";
  if (!GAME_init(&init)) {
    printf(fail_message);
  } else {
    // Load stuff
    GAME_load(&load);
    GAME_loop(&update, &draw);
  }

  GAME_quit(&quit);
  return 0;
}