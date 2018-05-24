#include "sprite.h"

extern Sprite* CreateSprite() {
  Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
  sprite->texture = NULL;
  sprite->clip_rect = NULL;
  sprite->animation = NULL;
  return sprite;
};

extern SDL_Rect* CreateClipRect(int x, int y, int w, int h) {
  SDL_Rect* rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  return rect;
}

extern void CreateSpriteAnimation(Sprite* sprite, int speed) {
  sprite->animation = (Animation*)malloc(sizeof(Animation));
  sprite->animation->speed = speed;
  sprite->animation->timestamp = 0;
  sprite->animation->current_frame = 0;
  sprite->animation->current_state = NULL;
  sprite->animation->states = NULL;
}

extern void DestroyAnimation(Animation* animation) {
  Animation_State* states = animation->states;
  if (states) {
    size_t i;
    for (i = 0; i < vector_size(states); ++i) {
      int* frames = states->frames;
      if (frames) {
        vector_free(frames);
      }
      frames = NULL;
    }
    vector_free(states);
  }
  animation = NULL;
}

extern void AddAnimationState(Sprite* sprite, char* id, int* frames) {
  Animation* animation = sprite->animation;
  if (animation != NULL) {
    Animation_State new_state = (Animation_State){id, frames};
    vector_push_back(animation->states, new_state);
  }
}

extern void PlaySpriteAnimation(Sprite* sprite, char* state_id) {
  Animation* animation = sprite->animation;
  if (animation) {
    Animation_State* states = animation->states;
    if (states) {
      size_t i = 0;
      Animation_State state;
      while (i < vector_size(states)) {
        if (states[i].id == state_id) {
          animation->current_state = &states[i];
        }
        i++;
      }
    }
  }
}

extern void UpdateSprite(Sprite* sprite, int delta) {
  Animation* animation = sprite->animation;
  if (animation != NULL) {
    int timestamp = animation->timestamp;
    int new_timestamp = timestamp + delta;
    if (new_timestamp >= animation->speed) {
      // Update timestamp and frame
      new_timestamp = 0;
      Animation_State* current_state = animation->current_state;
      if (animation->current_frame < vector_size(current_state->frames) - 1) {
        animation->current_frame++;
      } else {
        animation->current_frame = 0;
      }
      // Update rect to draw the new frame
      int frame = animation->current_state->frames[animation->current_frame];
      SDL_Rect* rect = sprite->clip_rect;
      int columns = sprite->texture->w / rect->w;
      int rows = sprite->texture->h / rect->h;
      int frame_row = frame / rows;
      int frame_column = frame % rows;
      rect->y = frame_column * rect->h;
      rect->x = frame_row * rect->w;
    }
    animation->timestamp = new_timestamp;
  }
}

extern void RenderSprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y) {
  int w, h;
  if (sprite->clip_rect == NULL) {
    w = sprite->texture->w;
    h = sprite->texture->h;
  } else {
    w = sprite->clip_rect->w;
    h = sprite->clip_rect->h;
  }
  SDL_Rect render_rect = (SDL_Rect){x, y, w, h};
  SDL_RenderCopy(renderer, sprite->texture->source_texture, sprite->clip_rect,
                 &render_rect);
}

extern void DestroySprite(Sprite* sprite) {
  if (sprite->clip_rect != NULL) {
    free(sprite->clip_rect);
    sprite->clip_rect = NULL;
  }
  if (sprite->clip_rect != NULL) {
    DestroyTexture(sprite->texture);
    sprite->texture = NULL;
  }
  if (sprite->animation != NULL) {
    DestroyAnimation(sprite->animation);
    sprite->animation = NULL;
  }
  free(sprite);
  sprite = NULL;
}