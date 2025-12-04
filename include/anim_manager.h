#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H
#include <SDL2/SDL.h>

typedef struct {
    int current_frame;
    Uint32 last_update_time;
    int frame_duration;
    int total_frames;
    int frame_width;
    int frame_height;
    SDL_Rect src_rect;
} Animation;

void Animation_Reset(Animation* anim);
void Animation_Init(Animation* anim, int frame_width, int frame_height, int total_frames, int frame_duration);
void Animation_Update(Animation* anim);
SDL_Rect* Animation_GetSourceRect(Animation* anim);

#endif // ANIM_MANAGER_H