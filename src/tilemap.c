#include "tilemap.h"

Tilemap* CreateTilemap(int x, int y, int w, int h, int tw, int th,
                       const char* tileset_path) {
  Tilemap* map = malloc(sizeof(Tilemap));
  map->x = x;
  map->y = y;
  map->w = w;
  map->h = h;
  map->tw = tw;
  map->th = th;
  map->tileset_texture = TextureFromFile(tileset_path);
  map->tiles = NULL;
};

void DestroyTilemap(Tilemap* map) {
  if (map->tileset_texture != NULL) {
    DestroyTexture(map->tileset_texture);
    map->tileset_texture = NULL;
  }
  if (map->tiles != NULL) {
    Entity** tiles = map->tiles;
    for (int i = 0; i < vector_size(tiles); i++) {
      DestroyEntity(tiles[i]);
      tiles[i] = NULL;
    }
    vector_free(tiles);
    tiles = NULL;
  }
  free(map);
  map = NULL;
};

void LoadMapDataFromFile(Tilemap* map, const char* path) {
  int* data = NULL;
  char c;

  // Read file
  FILE* file;
  file = fopen(path, "r");
  if (file) {
    bool valid = true;
    int line = 0;
    char** tokens = NULL;
    char* token = NULL;
    int token_index = 0;
    // Read file
    while ((c = getc(file)) != EOF) {
      switch (c) {
      case '\n':
        // Add last token
        vector_push_back(tokens, token);
        // Handle data according to line
        switch (line) {
        case 0:
          map->w = atoi(tokens[0]);
          map->h = atoi(tokens[1]);
          break;
        case 1:
          // Assign tokens to map data
          for (int i = 0; i < vector_size(tokens); i++) {
            vector_push_back(data, atoi(tokens[i]));
          };
          break;
        }
        vector_free(tokens);
        tokens = NULL;
        // Clean up
        token_index = 0;
        token = NULL;
        line++;
        break;
      case ',':
        // Reset token
        vector_push_back(tokens, token);
        token_index = 0;
        token = NULL;
        break;
      default:
        if (isdigit(c)) {
          vector_push_back(token, c);
          token_index++;
        }
        break;
      }
    }
    fclose(file);
  };

  // Map data array to entities
  if (data != NULL) {
    Entity** tiles;
    for (int i = 0; i < vector_size(data); i++) {
      int tile_row = data[i] / map->w;
      int tile_x_offset = tile_row;
      if (tile_row == 0) {
        tile_x_offset = data[i] - 1;
      }
      int tile_column = tile_x_offset % map->w;
      int tile_x = map->x + (i % map->w) * map->tw;
      int tile_y = map->y + (i / map->w) * map->th;
      Entity* e = CreateEntity(tile_x, tile_y);
      // TODO: may be animated (set accordingly)
      e->sprite = CreateSprite();
      e->sprite->texture = map->tileset_texture;
      e->sprite->clip_rect = CreateClipRect(
          tile_column * map->tw, tile_row * map->th, map->tw, map->th);

      // TODO: Set flags conditionally (extern function maybe)

      vector_push_back(map->tiles, e);
    }
  }
}

Entity* GetTile(Tilemap* map, int map_x, int map_y) {
  if (map->tiles != NULL) {
    return map->tiles[map->w * map_x + map_y];
  }
}

void DrawTilemap(SDL_Renderer* renderer, Tilemap* map) {
  Entity** tiles = map->tiles;
  if (tiles != NULL) {
    for (int i = 0; i < vector_size(tiles); i++) {
      DrawEntity(renderer, tiles[i]);
    }
  }
};