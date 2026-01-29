#ifndef WEAPONS_SYSTEM_H
#define WEAPONS_SYSTEM_H
#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* tex_bullet;
    SDL_Rect dest_bullet;

} Ammunitions;

bool CheckMunitions();

#endif