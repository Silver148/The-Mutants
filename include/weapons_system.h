#ifndef WEAPONS_SYSTEM_H
#define WEAPONS_SYSTEM_H
#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* tex_bullet;
    SDL_Rect dest_bullet;

} Ammunitions;

bool CheckMunitions();

/* Number of bullets granted by each ammunition box pickup */
#define AMMO_BOX_AMOUNT 60

#endif