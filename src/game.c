#include "game.h";
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#define FPS 60
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Screen dimension constants
static const int SCREEN_FPS = FPS;
static const int SCREEN_TICKS_PER_FRAME = 1000 / FPS;
static const RGBAColor SCREEN_BACKGROUND_COLOR = (RGBAColor){0, 0, 0, 255};
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 360;
static const Uint32 WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN_DESKTOP;

// SDL globals
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event e;
static InputState input_state = (InputState){false, false, false, false};

/// Loop functions ///

bool GAME_init(bool (*init_callback)()) {
  bool success = true;
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, WINDOW_FLAGS);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Init renderer
      const int window_width, window_height;
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      SDL_GetRendererOutputSize(renderer, &window_width, &window_height);
      if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      } else {
        // Calculate logical size
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Initialize renderer color
        SDL_SetRenderDrawColor(
            renderer, SCREEN_BACKGROUND_COLOR.r, SCREEN_BACKGROUND_COLOR.g,
            SCREEN_BACKGROUND_COLOR.b, SCREEN_BACKGROUND_COLOR.a);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
          printf("SDL_image could not initialize! SDL_image Error: %s\n",
                 IMG_GetError());
          success = false;
        }
      }
    }
  }
  success = init_callback();
  return success;
}

bool GAME_load(bool (*load_callback)()) { load_callback(); }

void GAME_loop(void (*update_callback)(InputState input, int delta),
               void (*draw_callback)(SDL_Renderer* renderer)) {
  bool quit_flag = false;
  Uint32 frame_start;
  Uint32 frame_time;
  int delta;
  while (!quit_flag) {
    frame_start = SDL_GetTicks();

    // Handle input
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit_flag = true;
      }
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    input_state = (InputState){false, false, false, false};
    if (currentKeyStates[SDL_SCANCODE_UP]) {
      input_state.up = true;
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
      input_state.down = true;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
      input_state.left = true;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
      input_state.right = true;
    }

    // Update
    update_callback(input_state, delta);

    // Draw
    SDL_RenderClear(renderer);
    const RGBAColor fill_color = (RGBAColor){0, 20, 30, 255};
    SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b,
                           fill_color.a);
    const SDL_Rect fill_rect = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &fill_rect);
    draw_callback(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, SCREEN_BACKGROUND_COLOR.r,
                           SCREEN_BACKGROUND_COLOR.g, SCREEN_BACKGROUND_COLOR.b,
                           SCREEN_BACKGROUND_COLOR.a);
    // Frame cap delay and delta calculation
    frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_time);
    }
    delta = SDL_GetTicks() - frame_start;
  }
}

void GAME_quit(void (*quit_callback)()) {
  quit_callback();
  // Destroy window
  SDL_DestroyWindow(window);
  window = NULL;
  // Quit SDL subsystems
  SDL_Quit();
  printf("Over and out.\n");
}

/// Utils ///
SDL_Texture* GAME_load_texture(const char* path) {
  // The final texture
  SDL_Texture* newTexture = NULL;
  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path);
  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL_image Error: %s\n", path,
           IMG_GetError());
  } else {
    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path,
             SDL_GetError());
    }
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}