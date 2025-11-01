#ifndef ZOMBIES_H
#define ZOMBIES_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    SDL_Surface* tmp_surf_zombie_idle;
    SDL_Texture* tex_zombie_idle;
    SDL_Rect src_zombie_idle;
    SDL_Rect dest_zombie_idle;
} IDLE_ZOMBIE;

typedef struct{
    SDL_Surface* tmp_surf_walk_zombie;
    SDL_Texture* tex_walk_zombie;
    SDL_Rect src_walk_zombie;
    SDL_Rect dest_walk_zombie;
} WALK_ZOMBIE;

typedef enum{
    IDLE_Z,
    WALK_Z
} StatesZombie;

void RenderZombieIdle();
void LoadSpritesZombies();

#endif