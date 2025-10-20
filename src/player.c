/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: player.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "player.h"
#include "global_vars.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PLAYER_IDLE_WIDTH 64
#define PLAYER_IDLE_HEIGHT 64

#define IDLE_FRAMES 2
#define FRAME_DURATION 150

/*IDLE PLAYER*/
IDLE_PLAYER idle_player;
int current_frame = 0;
Uint32 last_update_time = 0;

void LoadSpritesPlayer()
{   
    idle_player.tmp_surf_idleplayer = IMG_Load("sprites/idle_player_spritesheet.png");
    idle_player.tex_idleplayer = SDL_CreateTextureFromSurface(renderer, idle_player.tmp_surf_idleplayer);
    SDL_FreeSurface(idle_player.tmp_surf_idleplayer);
}

void AnimatePlayerShoot()
{
    //NOTHING YET :D
}

void Update_IDLE() {
    Uint32 current_time = SDL_GetTicks();

    if (current_time > last_update_time + FRAME_DURATION) {
        
        current_frame++;

        if (current_frame >= IDLE_FRAMES) {
            current_frame = 0;
        }

        last_update_time = current_time;
    }
}

void RenderIdlePlayerAnim()
{
    int src_x = current_frame * PLAYER_IDLE_WIDTH;
    idle_player.src_idleplayer.x = src_x;
    idle_player.src_idleplayer.y = 0;
    idle_player.src_idleplayer.w = PLAYER_IDLE_WIDTH;
    idle_player.src_idleplayer.h = PLAYER_IDLE_HEIGHT;

    idle_player.dest_idleplayer.x = 100;
    idle_player.dest_idleplayer.y = 100;
    idle_player.dest_idleplayer.w = PLAYER_IDLE_WIDTH * 2;
    idle_player.dest_idleplayer.h = PLAYER_IDLE_HEIGHT * 2;

    Update_IDLE();

    SDL_RenderCopy(renderer, idle_player.tex_idleplayer, &idle_player.src_idleplayer, &idle_player.dest_idleplayer);
}

void PlayerForward()
{
}

void PlayerBackward()
{
}

/*The_Light por favor manda el sprite de idle del playe a GitHub :)*/