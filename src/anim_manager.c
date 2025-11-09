/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Sebas-1423)
File: anim_manager.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/* ANIMATION MANAGER by Juan Yaguaro*/

#include "anim_manager.h"
#include <SDL2/SDL.h>

/*CURRENT FRAME AND LAST UPDATE TIME FOR ANIMATIONS*/
int current_frame = 0;
Uint32 last_update_time = 0;
int frame_duration = FRAME_DURATION;

void UpdateANIM(int frames) {
    Uint32 current_time = SDL_GetTicks();

    if (current_time > last_update_time + frame_duration) {
        
        current_frame++;

        if (current_frame >= frames) {
            current_frame = 0;
        }

        last_update_time = current_time;
    }
}