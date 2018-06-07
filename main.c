#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "src/entity.h"
#include "src/game.h"
#include "src/sprite.h"
#include "src/texture.h"
#include "src/tilemap.h"

// Test entity
Entity* thing = NULL;
Tilemap* map = NULL;
Entity* text = NULL;
Entity** entities = NULL;

// Setup
bool init() { return true; }

bool load() {
  // Test animated entity
  thing = CreateEntity(6, 0);
  thing->sprite = CreateSprite();
  thing->sprite->texture = TextureFromFile("assets/coqguy.png");
  thing->sprite->clip_rect = CreateClipRect(0, 0, 12, 24);
  CreateSpriteAnimation(thing->sprite, 400);
  int* frames = NULL;
  vector_push_back(frames, 0);
  vector_push_back(frames, 1);
  AddAnimationState(thing->sprite, "idle_right", frames);
  PlaySpriteAnimation(thing->sprite, "idle_right");

  // Test map
  // unsigned short* map_data = NULL;
  // for (int i = 0; i < 25; i++) {
  // vector_push_back(map_data, 9);
  //}
  map = CreateTilemap(0, 0, 5, 5, 24, 24, "assets/tileset.png");
  LoadMapDataFromFile(map, "data/maps/test.txt");

  // Test text
  text = CreateEntity(4, 146);
  text->sprite = CreateSprite();
  text->sprite->texture =
      TextureFromText("Test attribute: 40", (SDL_Color){255, 255, 255});

  // Group all entities
  vector_push_back(entities, thing);
  vector_push_back(entities, text);

  return true;
};

// Main loop
void update(InputState* input, int delta) {
  const float speed = 0.5f;
  const int move_distance = 24;
  if (input->up) {
    thing->y = thing->y - move_distance;
  }
  if (input->down) {
    thing->y = thing->y + move_distance;
  }
  if (input->left) {
    thing->x = thing->x - move_distance;
  }
  if (input->right) {
    thing->x = thing->x + move_distance;
  }
  // Update animation
  UpdateSprite(thing->sprite, delta);
}

void draw(SDL_Renderer* renderer) {
  // draw stuff
  DrawTilemap(renderer, map);
  for (int i = 0; i < vector_size(entities); i++) {
    DrawEntity(renderer, entities[i]);
  }
}

// Close and clean up
void quit() {
  for (int i = 0; i < vector_size(entities); i++) {
    DestroyEntity(entities[i]);
    entities[i] = NULL;
  }
  DestroyTilemap(map);
  // Clean up direct references
  map = NULL;
  thing = NULL;
  // Clean up vector
  vector_free(entities);
  entities = NULL;
}

int main(int argc, char* argv[]) {
  const char* fail_message = "Something failed. Killing.\n";
  if (!GAME_Init(&init)) {
    printf(fail_message);
  } else {
    // Load stuff
    GAME_Load(&load);
    GAME_Loop(&update, &draw);
  }

  GAME_Quit(&quit);
  return 0;
}