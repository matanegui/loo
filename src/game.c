#include "game.h";

#define FPS 60
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Screen dimension constants
static const int SCREEN_FPS = FPS;
static const int SCREEN_TICKS_PER_FRAME = 1000 / FPS;
static const SDL_Color SCREEN_BACKGROUND_COLOR = (SDL_Color){19, 18, 27};
static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 160;
// static const Uint32 WINDOW_FLAGS = 0;
static const Uint32 WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN_DESKTOP;

// SDL globals
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event e;

// Others
static TTF_Font* font = NULL;
static const char* FONT_PATH = "assets/fonts/fallout.ttf";
static const int FONT_SIZE = 12;
bool is_key_down = false;

/// Loop functions ///

bool GAME_Init(bool (*init_callback)()) {
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
        SDL_SetRenderDrawColor(renderer, SCREEN_BACKGROUND_COLOR.r,
                               SCREEN_BACKGROUND_COLOR.g,
                               SCREEN_BACKGROUND_COLOR.b, 255);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
          printf("SDL_image could not initialize! SDL_image Error: %s\n",
                 IMG_GetError());
          success = false;
        }

        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
          printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                 TTF_GetError());
          success = false;
        } else {
          font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
          if (font == NULL) {
            printf("Failed to load lazy font! SDL_ttf Error: %s\n",
                   TTF_GetError());
            success = false;
          }
        }
      }
    }
  }
  success = init_callback();
  return success;
}

bool GAME_Load(bool (*load_callback)()) { load_callback(); }

void GAME_Loop(void (*update_callback)(InputState* input, int delta),
               void (*draw_callback)(SDL_Renderer* renderer)) {
  bool quit_flag = false;
  Uint32 frame_start;
  Uint32 frame_time;
  int delta = 0;
  while (!quit_flag) {
    frame_start = SDL_GetTicks();

    // Handle input
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit_flag = true;
      } else if (e.type == SDL_KEYUP) {
        is_key_down = false;
      }
    }
    InputState input_state = (InputState){false, false, false, false};
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
      quit_flag = true;
    } else if (!is_key_down) {
      // Reset input state
      input_state.up = false;
      input_state.down = false;
      input_state.left = false;
      input_state.right = false;
      // Map keyboard press state to input state
      if (currentKeyStates[SDL_SCANCODE_UP]) {
        input_state.up = true;
        is_key_down = true;
      }
      if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        input_state.down = true;
        is_key_down = true;
      }
      if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        input_state.left = true;
        is_key_down = true;
      }
      if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        input_state.right = true;
        is_key_down = true;
      }
    }

    // Update
    update_callback(&input_state, delta);

    // Draw
    SDL_RenderClear(renderer);
    const SDL_Color fill_color = (SDL_Color){0, 20, 30};
    SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b,
                           255);
    const SDL_Rect fill_rect = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &fill_rect);
    draw_callback(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, SCREEN_BACKGROUND_COLOR.r,
                           SCREEN_BACKGROUND_COLOR.g, SCREEN_BACKGROUND_COLOR.b,
                           255);
    // Frame cap delay and delta calculation
    frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_time);
    }
    delta = SDL_GetTicks() - frame_start;
  }
}

void GAME_Quit(void (*quit_callback)()) {
  if (quit_callback != NULL) {
    quit_callback();
  }
  // Destroy window
  SDL_DestroyWindow(window);
  window = NULL;
  TTF_CloseFont(font);
  // Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  printf("Over and out.\n");
}

/// Utils ///

SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface) {
  SDL_Texture* new_texture = NULL;
  new_texture = SDL_CreateTextureFromSurface(renderer, surface);
  return new_texture;
}

SDL_Surface* CreateSurfaceFromPNG(const char* path) { return IMG_Load(path); }

SDL_Surface* CreateSurfaceFromText(const char* text, SDL_Color text_color) {
  return TTF_RenderText_Solid(font, text, text_color);
}