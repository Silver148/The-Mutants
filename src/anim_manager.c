#include "anim_manager.h"
#include <SDL2/SDL.h>

/*CURRENT FRAME AND LAST UPDATE TIME FOR ANIMATIONS*/
int current_frame = 0;
Uint32 last_update_time = 0;

void UpdateANIM(int frames) {
    Uint32 current_time = SDL_GetTicks();

    if (current_time > last_update_time + FRAME_DURATION) {
        
        current_frame++;

        if (current_frame >= frames) {
            current_frame = 0;
        }

        last_update_time = current_time;
    }
}