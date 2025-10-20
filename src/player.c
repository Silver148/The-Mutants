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

/*IDLE PLAYER*/
IDLE_PLAYER idle_player;
WALK_PLAYER walk_player;
int current_frame = 0;
Uint32 last_update_time = 0;

float position_x = 0;
float position_y = 0;

int last_position_x = 0;    
int last_position_y = 0;

StatesPlayer states_player;

float player_speed = 100.0f;

float deltaTime = 0;
Uint32 lastTime = 0;
Uint32 currentTime = 0;

void LoadSpritesPlayer()
{   
    idle_player.tmp_surf_idleplayer = IMG_Load("sprites/idle_player_spritesheet.png");
    idle_player.tex_idleplayer = SDL_CreateTextureFromSurface(renderer, idle_player.tmp_surf_idleplayer);
    SDL_FreeSurface(idle_player.tmp_surf_idleplayer);

    walk_player.tmp_surf_walkplayer = IMG_Load("sprites/walk_player_spritesheet.png");
    walk_player.tex_walkplayer = SDL_CreateTextureFromSurface(renderer, walk_player.tmp_surf_walkplayer);
    SDL_FreeSurface(walk_player.tmp_surf_walkplayer);
}

void UpdateDeltaTime()
{
    currentTime = SDL_GetTicks();
    deltaTime = (double)(currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
}

void AnimatePlayerShoot()
{
    //NOTHING YET :D
}

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

void RenderIdlePlayerAnim(SDL_RendererFlip flip_type)
{
    int idle_player_src_x = current_frame * PLAYER_WIDTH;
    idle_player.src_idleplayer.x = idle_player_src_x;
    idle_player.src_idleplayer.y = 0;
    idle_player.src_idleplayer.w = PLAYER_WIDTH;
    idle_player.src_idleplayer.h = PLAYER_HEIGHT;

    idle_player.dest_idleplayer.x = position_x;
    idle_player.dest_idleplayer.y = position_y;
    idle_player.dest_idleplayer.w = PLAYER_WIDTH;
    idle_player.dest_idleplayer.h = PLAYER_HEIGHT;

    SDL_RenderCopyEx(renderer, idle_player.tex_idleplayer, &idle_player.src_idleplayer, &idle_player.dest_idleplayer, 0.0, NULL, flip_type);
}

void PlayerForward()
{
    position_x += player_speed * deltaTime;
}

void PlayerWalkAnim(SDL_RendererFlip flip_type)
{
    SDL_RenderCopyEx(renderer, walk_player.tex_walkplayer, &walk_player.src_walkplayer, 
    &walk_player.dest_walkplayer, 0.0, NULL, flip_type);
}

void RenderPlayer(SDL_RendererFlip flip_type)
{
    int walk_player_src_x = current_frame * PLAYER_WIDTH;

    walk_player.src_walkplayer.x = walk_player_src_x;
    walk_player.src_walkplayer.y = 0;
    walk_player.src_walkplayer.w = PLAYER_WIDTH;
    walk_player.src_walkplayer.h = PLAYER_HEIGHT;

    walk_player.dest_walkplayer.x = position_x;
    walk_player.dest_walkplayer.y = position_y;
    walk_player.dest_walkplayer.w = PLAYER_WIDTH;
    walk_player.dest_walkplayer.h = PLAYER_HEIGHT;

    if(states_player == IDLE){
        RenderIdlePlayerAnim(flip_type);
    }else if(states_player == WALK){
        PlayerWalkAnim(flip_type);
    }
}

void PlayerBackward()
{
    position_x -= player_speed * deltaTime;
}