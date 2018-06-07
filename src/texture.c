#include "texture.h"

extern Texture* CreateTexture() {
  Texture* texture = malloc(sizeof(Texture));
  texture->source_texture = NULL;
  texture->w = 0;
  texture->h = 0;
  return texture;
}

extern void DestroyTexture(Texture* texture) {
  SDL_DestroyTexture(texture->source_texture);
  texture->source_texture = NULL;
  free(texture);
}

extern Texture* TextureFromFile(const char* path) {
  Texture* texture = CreateTexture();
  SDL_Texture* source_texture = NULL;
  // Load image at specified path
  SDL_Surface* surface = CreateSurfaceFromPNG(path);
  if (surface == NULL) {
    printf("Unable to load image %s! SDL_image Error.");
  } else {
    // Create texture from surface pixels
    source_texture = CreateTextureFromSurface(surface);
    if (source_texture == NULL) {
      printf("Unable to create texture from %s! SDL Error");
    } else {
      texture = CreateTexture();
      texture->source_texture = source_texture;
      texture->w = surface->w;
      texture->h = surface->h;
    }
    // Get rid of old loaded surface
    SDL_FreeSurface(surface);
  }
  return texture;
}

extern Texture* TextureFromText(const char* text, SDL_Color text_color) {
  Texture* texture = CreateTexture();
  SDL_Texture* source_texture = NULL;

  // Render text surface
  SDL_Surface* surface = CreateSurfaceFromText(text, text_color);
  if (surface == NULL) {
    printf("Unable to render text surface! SDL_ttf Error.");
  } else {
    // Create texture from surface pixels
    source_texture = CreateTextureFromSurface(surface);
    if (source_texture == NULL) {
      printf("Unable to create texture from rendered text! SDL Error.");
    } else {
      texture->source_texture = source_texture;
      texture->w = surface->w;
      texture->h = surface->h;
    }
    // Get rid of old surface
    SDL_FreeSurface(surface);
  }
  // Return success
  return texture;
}
