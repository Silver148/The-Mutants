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
#include "zombie_waves.h"
#include <SDL2/SDL.h>

/* camera source rect from state_game.c */
extern SDL_Rect backgroundSrcRect;
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* reuse the existing sprite containers */
IDLE_ZOMBIE idle_zombie;
WALK_ZOMBIE walk_zombie;

ZOMBIE zombies[MAX_ZOMBIES];
int num_zombies = 0;
int next_zombie_id = 0;

/* Animation frames for zombies */
#define ZOMBIE_IDLE_FRAMES 4
#define ZOMBIE_WALK_FRAMES 4
#define FRAME_DURATION_ZOMBIE 150  // ms por frame

/* external globals from other modules */
extern float deltaTime;       /* provided by delta_time.c */
extern int health;            /* player health (player.c) */

int wave_counter = 0;

#define INITIAL_ZOMBIES 3
#define SPAWN_COOLDOWN 5.0f


static int find_free_zombie_slot() {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].alive) {
            return i;
        }
    }
    return -1;
}

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

static void get_random_spawn_position(float* x, float* y) {
    float player_x = GetPositionPlayerX();
    
    do {
        *x = 50 + rand() % (640 - 100);
    } while (fabsf(*x - player_x) < 100.0f); 
    
    *y = 350.0f;
}

int SpawnZombie(float x, float y) {
    int slot = find_free_zombie_slot();
    if (slot == -1) {
        printf("The maximum number of zombies has already spawned!\n");
        return -1;
    }
    
    ZOMBIE* z = &zombies[slot];
    
    z->x = x;
    z->y = 350.0f;
    z->base_y = 350.0f;
    z->speed = 30.0f + (rand() % 20);
    z->dir = (rand() % 2) ? 1 : -1;
    z->wander_timer = 0.5f + (rand() % 200) / 100.0f;
    z->state = IDLE_Z;
    
    z->dest.w = ZOMBIE_WIDTH;
    z->dest.h = ZOMBIE_HEIGHT;
    
    z->attack_cooldown = 1.0f + (rand() % 100) / 100.0f;
    z->attack_timer = 0.0f;
    z->attack_damage = 5 + (rand() % 10);
    z->health = 100 + (rand() % 50);
    z->alive = 1;
    z->id = next_zombie_id++;
    
    num_zombies++;
    printf("Zombie %d spawn in (%.1f, %.1f) HP: %d\n", 
           z->id, x, y, z->health);
    
    return z->id;
}

void SpawnZombieRandom() {
    float x, y;
    get_random_spawn_position(&x, &y);
    SpawnZombie(x, y);
}

void InitZombieSys()
{
    num_zombies = 0;
    next_zombie_id = 0;

    /*INICIALIZAR ZOMBIES*/
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        zombies[i].alive = 0;
        zombies[i].id = 0;
    }
}

void KillZombie(int zombie_id) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombies[i].alive && zombies[i].id == zombie_id) {
            zombies[i].alive = 0;
            num_zombies--;
            printf("Zombie %d killed. Remaining Zombies: %d\n", 
                   zombie_id, num_zombies);
            break;
        }
    }
}

static void zombie_chase_player(ZOMBIE* z, float dx, float dy, float dist) {
    if(fabsf(dx) <= 0.5f) return;

    float nx = (dx < 0.0f) ? -1.0f : 1.0f;
    z->x += nx * z->speed * deltaTime;
    z->y = z->base_y;
    z->state = WALK_Z;
    z->dir = (nx < 0.0f) ? -1 : 1;
}

static void zombie_wander(ZOMBIE* z) {
    z->wander_timer -= deltaTime;
    if(z->wander_timer <= 0.0f){
        int r = rand() % 3;
        z->dir = r - 1;
        z->wander_timer = 0.5f + (rand() % 200) / 100.0f;
    }

    if(z->dir != 0){
        z->x += z->dir * z->speed * deltaTime * 0.6f;
        z->state = WALK_Z;
    } else {
        z->state = IDLE_Z;
    }
    z->y = z->base_y;
}

int current_wave = 0;

void UpdateZombies() {
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();

    static WAVE wave1, wave2;
    static int wave_state = 0;
    ZOMBIE* z = NULL;

    for(int i = 0; i<MAX_ZOMBIES; i++)
    {
        if(zombies[i].alive)
        {
            z = &zombies[i];
            break;
        }
    }

    if(!z)
    {
        z = &zombies[0];
    }

    if(wave_state == 0)
    {
        InitWave(&wave1, 20, 2.0f, 5);
        wave_state = 1;
        SDL_Log("STARTING WAVE 1");
    }

    if(wave_state == 1)
    {
        int r = UpdateWave(&wave1, z);

        if(r == -1)
        {
            wave_state = 2;
            SDL_Log("WAVE 1 FINISHED");
        }
    }

    if(wave_state == 2)
    {
        InitWave(&wave2, 40, 2 + rand() % 2, 10);

        for(int i = 0; i<MAX_ZOMBIES; i++)
        {
            zombies[i].id = 0;
            next_zombie_id = 0;
        }

        SDL_Log("STARTING WAVE 2");
        wave_state = 3;
    }

    if(wave_state == 3)
    {
        int r = UpdateWave(&wave2, z);

        if(r == -1)
        {
            SDL_Log("WAVE 2 FINISHED");
            SDL_Log("GAME FINISHED!!!");
            wave_state = -1;
        }
    }
    
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        ZOMBIE* z = &zombies[i];
        if (!z->alive) continue;
        
        float dx = player_x - z->x;
        float dy = player_y - z->y;
        float dist = sqrtf(dx*dx + dy*dy);
        
        if(dist <= ZOMBIE_DETECT_RADIUS){
            zombie_chase_player(z, dx, dy, dist);
        } else {
            zombie_wander(z);
        }
        
        if(z->attack_timer > 0.0f){
            z->attack_timer -= deltaTime;
            if(z->attack_timer < 0.0f) z->attack_timer = 0.0f;
        }
        
        if(z->x < 0) z->x = 0;
        if(z->x > 640 - ZOMBIE_WIDTH) z->x = 640 - ZOMBIE_WIDTH;
        if(z->base_y < 0) z->base_y = 0;
        if(z->base_y > 480 - ZOMBIE_HEIGHT) z->base_y = 480 - ZOMBIE_HEIGHT;
        z->y = z->base_y;
        
        z->dest.x = (int)z->x;
        z->dest.y = (int)z->base_y;
        
        Hitbox player_hitbox = GetPlayerHitbox();
        SDL_Rect zrect = { 
            z->dest.x + ZOMBIE_HITBOX_OFFSET_X, 
            z->dest.y + ZOMBIE_HITBOX_OFFSET_Y, 
            ZOMBIE_HITBOX_WIDTH, 
            ZOMBIE_HITBOX_HEIGHT 
        };
        SDL_Rect prect = { 
            (int)player_hitbox.x, 
            (int)player_hitbox.y, 
            player_hitbox.w, 
            player_hitbox.h 
        };
        
        if (zrect.x < prect.x + prect.w && zrect.x + zrect.w > prect.x &&
            zrect.y < prect.y + prect.h && zrect.y + zrect.h > prect.y) {
            if(z->attack_timer <= 0.0f){
                health -= z->attack_damage;
                if(health < 0) health = 0;
                z->attack_timer = z->attack_cooldown;
                SDL_Log("Player hit by zombie %d! Health: %d\n", 
                       z->id, health);
            }
        }
        
        if (z->health <= 0) {
            KillZombie(z->id);
        }
    }
}


void RenderZombies() {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        ZOMBIE* z = &zombies[i];
        if (!z->alive) continue;
        
        SDL_Texture* tex = NULL;
        SDL_Rect* src_rect = NULL;
        
        if(z->state == IDLE_Z){
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
        
        if(tex == NULL || src_rect == NULL) continue;
        
        SDL_Rect drawDest = z->dest;
        drawDest.x -= backgroundSrcRect.x;
        drawDest.y -= backgroundSrcRect.y;
        SDL_RenderCopyEx(renderer, tex, src_rect, &drawDest, 0.0, NULL, 
                (z->dir < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        
        //Bar Zombie health
        if (z->health < 100) {
            SDL_Rect health_bg = {drawDest.x, drawDest.y - 10, ZOMBIE_WIDTH, 5};
            SDL_Rect health_fg = {drawDest.x, drawDest.y - 10, 
                                 (ZOMBIE_WIDTH * z->health) / 100, 5};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &health_bg);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &health_fg);
        }
    }
}

void ShowHitboxZombie(int zombie_index) {
    if (zombie_index < 0 || zombie_index >= MAX_ZOMBIES || 
        !zombies[zombie_index].alive) return;
    
    ZOMBIE* z = &zombies[zombie_index];
    SDL_Rect hitboxRect = { 
        z->dest.x + ZOMBIE_HITBOX_OFFSET_X - backgroundSrcRect.x, 
        z->dest.y + ZOMBIE_HITBOX_OFFSET_Y - backgroundSrcRect.y, 
        ZOMBIE_HITBOX_WIDTH, 
        ZOMBIE_HITBOX_HEIGHT 
    };

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &hitboxRect);
}

void CleanupZombieSystem() {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        zombies[i].alive = 0;
        zombies[i].id = 0;
    }
    num_zombies = 0;
}

/* kept for compatibility with existing calls */
void RenderZombieIdle()
{
    /* simple wrapper that renders the zombie at its current position */
    RenderZombies();
}
