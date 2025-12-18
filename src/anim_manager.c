/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: anim_manager.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/* ANIMATION MANAGER by Juan Yaguaro*/

#include "anim_manager.h"
#include <SDL2/SDL.h>

void Animation_Init(Animation* anim, int frame_width, int frame_height, int total_frames, int frame_duration) {
    anim->current_frame = 0;
    anim->last_update_time = SDL_GetTicks();
    anim->frame_duration = frame_duration;
    anim->total_frames = total_frames;
    anim->frame_width = frame_width;
    anim->frame_height = frame_height;
    
    anim->src_rect.x = 0;
    anim->src_rect.y = 0;
    anim->src_rect.w = frame_width;
    anim->src_rect.h = frame_height;

#ifdef ANIM_DEBUG
    SDL_Log("Animation_Init: frames=%d duration=%d pw=%d ph=%d\n", total_frames, frame_duration, frame_width, frame_height);
#endif
}

void Animation_Update(Animation* anim) {
    Uint32 current_time = SDL_GetTicks();
    
    if (current_time > anim->last_update_time + anim->frame_duration) {
        anim->current_frame++;
        
        if (anim->current_frame >= anim->total_frames) {
            anim->current_frame = 0;
        }
        
        anim->src_rect.x = anim->current_frame * anim->frame_width;
        anim->last_update_time = current_time;

    #ifdef ANIM_DEBUG
        SDL_Log("Animation_Update: current_frame=%d src_x=%d\n", anim->current_frame, anim->src_rect.x);
    #endif
    }
}

void Animation_Reset(Animation* anim) {
    anim->current_frame = 0;
    anim->src_rect.x = 0;
    anim->last_update_time = SDL_GetTicks();
}

SDL_Rect* Animation_GetSourceRect(Animation* anim) {
    return &anim->src_rect;
}