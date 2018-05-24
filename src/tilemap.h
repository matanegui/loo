#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../libs/vector.h"

#include "entity.h"
#include "texture.h"

#pragma once

typedef struct {
  int x, y;
  int w, h;   // Tilemap width and height (in tiles)
  int tw, th; // Tile height and width (for offset calculation)
  Texture* tileset_texture;
  Entity** tiles;
} Tilemap;

extern Tilemap* CreateTilemap(int x, int y, int w, int h, int tw, int th,
                              const char* tileset_path);
extern void DestroyTilemap(Tilemap* map);
extern void LoadMapDataFromFile(Tilemap* map, const char* path);
extern Entity* GetTile(Tilemap* map, int map_x, int map_y);
extern void DrawTilemap(SDL_Renderer* renderer, Tilemap* map);