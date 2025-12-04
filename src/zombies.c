/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: zombies.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/* ZOMBIES MANAGEMENT by Juan Yaguaro and Abel Ferrer*/
#include "zombies.h"
#include "global_vars.h"
#include "player.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* reuse the existing sprite containers */
IDLE_ZOMBIE idle_zombie;
WALK_ZOMBIE walk_zombie;

/* single zombie instance for now */
ZOMBIE zombie;

/* tuning */
#define ZOMBIE_WIDTH 64
#define ZOMBIE_HEIGHT 64
#define ZOMBIE_DETECT_RADIUS 180.0f

/* Animation frames for zombies */
#define ZOMBIE_IDLE_FRAMES 4
#define ZOMBIE_WALK_FRAMES 4
#define FRAME_DURATION_ZOMBIE 150  // ms por frame

/* external globals from other modules */
extern float deltaTime;       /* provided by delta_time.c */
extern float position_x;      /* player position X (player.c) */
extern float position_y;      /* player position Y (player.c) */
extern int health;            /* player health (player.c) */

void LoadSpritesZombies()
{
    /* load idle */
    idle_zombie.tmp_surf_zombie_idle = IMG_Load("sprites/idle_zombie-spritesheet.png");
    if(idle_zombie.tmp_surf_zombie_idle){
        idle_zombie.tex_zombie_idle = SDL_CreateTextureFromSurface(renderer, idle_zombie.tmp_surf_zombie_idle);
        SDL_FreeSurface(idle_zombie.tmp_surf_zombie_idle);
        
        // Inicializar animación idle
        Animation_Init(&idle_zombie.idle_anim, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, 
                      ZOMBIE_IDLE_FRAMES, FRAME_DURATION_ZOMBIE);
    } else {
        fprintf(stderr, "Failed to load idle_zombie sprite\n");
        idle_zombie.tex_zombie_idle = NULL;
    }

    /* load walk */
    walk_zombie.tmp_surf_walk_zombie = IMG_Load("sprites/walk_zombie_spritesheet.png");
    if(walk_zombie.tmp_surf_walk_zombie){
        walk_zombie.tex_walk_zombie = SDL_CreateTextureFromSurface(renderer, walk_zombie.tmp_surf_walk_zombie);
        SDL_FreeSurface(walk_zombie.tmp_surf_walk_zombie);
        
        // Inicializar animación walk
        Animation_Init(&walk_zombie.walk_anim, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, 
                      ZOMBIE_WALK_FRAMES, FRAME_DURATION_ZOMBIE);
    } else {
        fprintf(stderr, "Failed to load walk_zombie sprite\n");
        walk_zombie.tex_walk_zombie = NULL;
    }
}

void InitZombie(float x, float y)
{
    /* seed random once */
    static int seeded = 0;
    if(!seeded){
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    zombie.x = x;
    zombie.y = y;
    zombie.base_y = y; /* keep a reference for ground level */
    zombie.speed = 40.0f; /* px/sec, tweak as needed */
    zombie.dir = -1;
    zombie.wander_timer = 0.5f + (rand()%200)/100.0f; /* 0.5 - 2.5s */
    zombie.state = IDLE_Z;

    zombie.dest.w = ZOMBIE_WIDTH;
    zombie.dest.h = ZOMBIE_HEIGHT;
    
    /* attack defaults */
    zombie.attack_cooldown = 1.0f; /* 1 second between hits */
    zombie.attack_timer = 0.0f;
    zombie.attack_damage = 10;
}

static void zombie_chase_player(float dx, float dy, float dist)
{
    /* Chase only horizontally to avoid 'flying' when player jumps */
    if(fabsf(dx) <= 0.5f) return; /* already close on X */

    float nx = (dx < 0.0f) ? -1.0f : 1.0f;
    zombie.x += nx * zombie.speed * deltaTime;

    /* keep zombie on the ground level */
    zombie.y = zombie.base_y;

    zombie.state = WALK_Z;
    zombie.dir = (nx < 0.0f) ? -1 : 1;
}

static void zombie_wander()
{
    zombie.wander_timer -= deltaTime;
    if(zombie.wander_timer <= 0.0f){
        /* pick new direction: -1, 0, 1 */
        int r = rand() % 3; /* 0,1,2 */
        zombie.dir = r - 1;
        zombie.wander_timer = 0.5f + (rand()%200)/100.0f; /* 0.5 - 2.5s */
    }

    if(zombie.dir != 0){
        zombie.x += zombie.dir * zombie.speed * deltaTime * 0.6f; /* wander slower */
        zombie.state = WALK_Z;
    } else {
        zombie.state = IDLE_Z;
    }
    /* ensure zombie stays on ground */
    zombie.y = zombie.base_y;
}

void UpdateZombies()
{
    /* compute vector to player */
    float dx = position_x - zombie.x;
    float dy = position_y - zombie.y;
    float dist = sqrtf(dx*dx + dy*dy);

    if(dist <= ZOMBIE_DETECT_RADIUS){
        /* chase player */
        zombie_chase_player(dx, dy, dist);
    } else {
        /* wander */
        zombie_wander();
    }

    /* update attack timer */
    if(zombie.attack_timer > 0.0f){
        zombie.attack_timer -= deltaTime;
        if(zombie.attack_timer < 0.0f) zombie.attack_timer = 0.0f;
    }

    /* screen bounds clamp (basic) assuming 640x480 playfield; adjust if different */
    if(zombie.x < 0) zombie.x = 0;
    if(zombie.x > 640 - ZOMBIE_WIDTH) zombie.x = 640 - ZOMBIE_WIDTH;
    /* keep vertical position clamped around base_y */
    if(zombie.base_y < 0) zombie.base_y = 0;
    if(zombie.base_y > 480 - ZOMBIE_HEIGHT) zombie.base_y = 480 - ZOMBIE_HEIGHT;
    zombie.y = zombie.base_y;

    /* update dest rect for rendering */
    zombie.dest.x = (int)zombie.x;
    zombie.dest.y = (int)zombie.base_y;

    /* simple AABB collision with player */
    SDL_Rect zrect = { zombie.dest.x, zombie.dest.y, zombie.dest.w, zombie.dest.h };
    SDL_Rect prect = { (int)position_x, (int)position_y, PLAYER_WIDTH, PLAYER_HEIGHT };

    if (zrect.x < prect.x + prect.w && zrect.x + zrect.w > prect.x &&
        zrect.y < prect.y + prect.h && zrect.y + zrect.h > prect.y)
    {
        /* collision */
        if(zombie.attack_timer <= 0.0f){
            health -= zombie.attack_damage;
            if(health < 0) health = 0;
            zombie.attack_timer = zombie.attack_cooldown;
            SDL_Log("Player hit by zombie! Health=%d\n", health);
        }
    }
}

void RenderZombies()
{
    SDL_Texture* tex = NULL;
    SDL_Rect* src_rect = NULL;
    
    if(zombie.state == IDLE_Z){
        tex = idle_zombie.tex_zombie_idle;
        if (tex != NULL) {
            Animation_Update(&idle_zombie.idle_anim);
            src_rect = Animation_GetSourceRect(&idle_zombie.idle_anim);
        }
    } else {
        tex = walk_zombie.tex_walk_zombie;
        if (tex != NULL) {
            Animation_Update(&walk_zombie.walk_anim);
            src_rect = Animation_GetSourceRect(&walk_zombie.walk_anim);
        }
    }

    if(tex == NULL || src_rect == NULL) return; /* nothing to draw */

    SDL_RenderCopyEx(renderer, tex, src_rect, &zombie.dest, 0.0, NULL, 
                    (zombie.dir < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

/* kept for compatibility with existing calls */
void RenderZombieIdle()
{
    /* simple wrapper that renders the zombie at its current position */
    RenderZombies();
}