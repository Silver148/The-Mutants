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

#define POS_Y 250
#define POS_X 100

/*IDLE PLAYER*/
IDLE_PLAYER idle_player;

/*WALK PLAYER*/
WALK_PLAYER walk_player;

/*CURRENT FRAME AND LAST UPDATE TIME FOR ANIMATIONS*/
int current_frame = 0;
Uint32 last_update_time = 0;

/*JUMP PLAYER*/
JUMP_PLAYER jump_player;

/*POSITION X AND Y OF THE PLAYER*/
float position_x = POS_X;
float position_y = POS_Y;

int last_position_x = 0;    
int last_position_y = 0;

int health = 150; 

/*STATES OF THE PLAYER*/
StatesPlayer states_player;

/* control de velocidad(boost) */
void SetPlayerSpeedMultiplier(float multiplier);
void ResetPlayerSpeed(void);

/*PLAYER SPEED*/
/* Base and current player speed */
static float base_player_speed = 60.0f;
float player_speed = 60.0f;

/*DELTA TIMES*/
float deltaTime = 0;
Uint32 lastTime = 0;
Uint32 currentTime = 0;

/*JUMP VARIABLES*/
bool is_jumping = false;
float jump_force = 250.0f;
float gravity = 900.0f;
float velocity_y = 0.0f;


void LoadSpritesPlayer()
{   
    idle_player.tmp_surf_idleplayer = IMG_Load("sprites/idle_player_spritesheet.png");
    idle_player.tex_idleplayer = SDL_CreateTextureFromSurface(renderer, idle_player.tmp_surf_idleplayer);
    SDL_FreeSurface(idle_player.tmp_surf_idleplayer);

    walk_player.tmp_surf_walkplayer = IMG_Load("sprites/walk_player_spritesheet.png");
    walk_player.tex_walkplayer = SDL_CreateTextureFromSurface(renderer, walk_player.tmp_surf_walkplayer);
    SDL_FreeSurface(walk_player.tmp_surf_walkplayer);

    jump_player.tmp_surf_jumpplayer = IMG_Load("sprites/jump_player-spritesheet.png");
    jump_player.tex_jumpplayer = SDL_CreateTextureFromSurface(renderer, jump_player.tmp_surf_jumpplayer);
    SDL_FreeSurface(jump_player.tmp_surf_jumpplayer);

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

void PlayerJumpAnim(SDL_RendererFlip flip_type)
{
    SDL_RenderCopyEx(renderer, jump_player.tex_jumpplayer, &jump_player.src_jumpplayer, 
    &jump_player.dest_jumpplayer, 0.0, NULL, flip_type);
}

void RenderPlayer(SDL_RendererFlip flip_type)
{
    int walk_player_src_x = current_frame * PLAYER_WIDTH;
    int jump_player_src_x = current_frame * PLAYER_WIDTH;

    walk_player.src_walkplayer.x = walk_player_src_x;
    walk_player.src_walkplayer.y = 0;
    walk_player.src_walkplayer.w = PLAYER_WIDTH;
    walk_player.src_walkplayer.h = PLAYER_HEIGHT;

    walk_player.dest_walkplayer.x = position_x;
    walk_player.dest_walkplayer.y = position_y;
    walk_player.dest_walkplayer.w = PLAYER_WIDTH;
    walk_player.dest_walkplayer.h = PLAYER_HEIGHT;

    jump_player.src_jumpplayer.x = jump_player_src_x;
    jump_player.src_jumpplayer.y = 0;
    jump_player.src_jumpplayer.w = PLAYER_WIDTH;
    jump_player.src_jumpplayer.h = PLAYER_HEIGHT;

    jump_player.dest_jumpplayer.x = position_x;
    jump_player.dest_jumpplayer.y = position_y;
    jump_player.dest_jumpplayer.w = PLAYER_WIDTH;
    jump_player.dest_jumpplayer.h = PLAYER_HEIGHT;

    if(states_player == IDLE){
        RenderIdlePlayerAnim(flip_type);
    }else if(states_player == WALK){
        PlayerWalkAnim(flip_type);
    } else if(states_player == JUMP){
        PlayerJumpAnim(flip_type);
    }
}

void PlayerJump()
{
    velocity_y += gravity * deltaTime; 

    position_y += velocity_y * deltaTime;

    if(position_y >= POS_Y)
    {
        position_y = POS_Y;
        is_jumping = false;
        velocity_y = 0.0f;
    }
}

void PlayerBackward()
{
    position_x -= player_speed * deltaTime;
}

/*
 * Set a multiplier for player speed. multiplier = 1.0 -> normal speed.
 * Example: SetPlayerSpeedMultiplier(1.8f) to increase by 80%.
 */
void SetPlayerSpeedMultiplier(float multiplier)
{
    if (multiplier <= 0.0f) {
        /* avoid invalid speeds */
        player_speed = base_player_speed;
    } else {
        player_speed = base_player_speed * multiplier;
    }
}

/* restaura la velocidad del jugador a la normal*/
void ResetPlayerSpeed(void)
{
    player_speed = base_player_speed;
}
