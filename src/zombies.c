/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: zombies.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "zombies.h"
#include "global_vars.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

IDLE_ZOMBIE idle_zombie;

void LoadSpritesZombies()
{
    idle_zombie.tmp_surf_zombie_idle = IMG_Load("sprites/idle_zombie-spritesheet.png");
    idle_zombie.tex_zombie_idle = SDL_CreateTextureFromSurface(renderer, idle_zombie.tmp_surf_zombie_idle);
    SDL_FreeSurface(idle_zombie.tmp_surf_zombie_idle);

     walk_zombie.tmp_surf_walkzombie = IMG_Load("sprites/walk_zombie_spritesheet.png");
     walk_zombie.tex_walkzombie = SDL_CreateTextureFromSurface(renderer,  walk_zombie.tmp_surf_walkzombie);
    SDL_FreeSurface(walk_zombie.tmp_surf_walkzombie);

}

void RenderZombieIdle()
{
    idle_zombie.src_zombie_idle.x = 0;
    idle_zombie.src_zombie_idle.y = 0;
    idle_zombie.src_zombie_idle.w = 64;
    idle_zombie.src_zombie_idle.h = 64;

    idle_zombie.dest_zombie_idle.x = 150;
    idle_zombie.dest_zombie_idle.y = 250;
    idle_zombie.dest_zombie_idle.w = 64;
    idle_zombie.dest_zombie_idle.h = 64;

    SDL_RenderCopyEx(renderer, idle_zombie.tex_zombie_idle, &idle_zombie.src_zombie_idle, &idle_zombie.dest_zombie_idle, 0.0, NULL, SDL_FLIP_NONE);

    
}
