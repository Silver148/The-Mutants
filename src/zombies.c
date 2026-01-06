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
#include "system_cinematics.h"
#include "states.h"
#include "music.h"
#include "projectiles.h"
#include <SDL2/SDL.h>

/* camera source rect from state_game.c */
extern SDL_Rect backgroundSrcRect;
extern int backgroundImgW;
extern int backgroundImgH;
/* allow changing background from other modules */
extern void SetBackgroundImage(const char* path);
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* file-scope state so all functions can access it */
static int wave_state = 0;
static SDL_Surface* tmp_surf_winner = NULL;
static SDL_Texture* winner_tex = NULL;
/* when wave2 finishes, wait until all zombies are cleared before changing bg */
static int pending_bg_change = 0;

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

extern TTF_Font* font_kills;

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

    /* load winner image (shown when both waves finish) */
    tmp_surf_winner = IMG_Load("sprites/winner.png");
    if (tmp_surf_winner) {
        winner_tex = SDL_CreateTextureFromSurface(renderer, tmp_surf_winner);
        SDL_FreeSurface(tmp_surf_winner);
        tmp_surf_winner = NULL;
    } else {
        fprintf(stderr, "Failed to load winner sprite\n");
        winner_tex = NULL;
    }
}

static void get_random_spawn_position(float* x, float* y) {
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();

    int margin = 100; /* minimum distance from player on X */
    int min_x = 50;
    int max_x = backgroundImgW - 50;
    if (max_x <= min_x) max_x = min_x + 1;

    do {
        *x = (float)(min_x + rand() % (max_x - min_x));
    } while (fabsf(*x - player_x) < (float)margin);

    /* spawn so zombie feet align with player feet */
    *y = player_y + PLAYER_HEIGHT - ZOMBIE_HEIGHT;
}

int SpawnZombie(float x, float y) {
    int slot = find_free_zombie_slot();
    if (slot == -1) {
        printf("The maximum number of zombies has already spawned!\n");
        return -1;
    }
    
    ZOMBIE* z = &zombies[slot];
    z->x = x;
    z->y = y;
    z->base_y = y;
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
        z->dest.x = (int)z->x;
        z->dest.y = (int)z->base_y;

        /* debug: always log spawn Y to help trace vertical issues */
        SDL_Log("SpawnZombie: id=%d world=(%.1f,%.1f) base_y=%.1f dest_y=%d HP=%d", z->id, x, y, z->base_y, z->dest.y, z->health);

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

    if(z->x >= backgroundImgW - ZOMBIE_WIDTH) {
        z->x = backgroundImgW - ZOMBIE_WIDTH;
        z->dir = -1;
    }
}

int current_wave = 0;
/* current_wave is kept for compatibility with other modules */

void UpdateZombies() {
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();

    static WAVE wave1, wave2, wave3, wave4;
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
        InitWave(&wave1, 10, 2.0f, 5);
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
        InitWave(&wave2, 10, 2 + rand() % 2, 10);

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
            /* mark that initial waves finished; wait until all zombies are cleared */
            wave_state = 4; /* waiting-for-clear state */
            pending_bg_change = 1;
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
        
        extern int backgroundImgW;
        extern int backgroundImgH;
        if(z->x < 0) z->x = 0;
        if(z->base_y < 0) z->base_y = 0;
        if(z->base_y > backgroundImgH - ZOMBIE_HEIGHT) z->base_y = 350;
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

    /* If waves finished earlier and we're waiting for all zombies to be cleared,
       perform the background change only once there are no active zombies. */
    if (pending_bg_change && num_zombies == 0) {
        InitSystemCinematics();
        PlayCinematic("cinematics/win1.mp4", renderer);
        ShutdownCinematicsSystem();
        SDL_RenderClear(renderer);

        SDL_Log("All zombies cleared after waves. Changing background now.");
        /* change background, then start two additional waves */
        SetBackgroundImage("sprites/113 sin título_20260104134157~2.png");
        pending_bg_change = 0;
        /* start wave 3 */
        InitWave(&wave3, 10, 2 + rand() % 2, 8);
        for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
        SDL_Log("STARTING WAVE 3");
        wave_state = 5; /* wave3 running */
    }

    /* Additional waves logic (wave3 -> wave4 -> final) */
    if (wave_state == 5) {
        int r = UpdateWave(&wave3, z);
        if (r == -1) {
            SDL_Log("WAVE 3 FINISHED");
            /* init wave4 */
            InitWave(&wave4, 10, 2 + rand() % 2, 12);
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
            SDL_Log("STARTING WAVE 4");
            wave_state = 6; /* wave4 running */
        }
    }

    if (wave_state == 6) {
        int r = UpdateWave(&wave4, z);
        if (r == -1) {
            SDL_Log("WAVE 4 FINISHED");
            SDL_Log("ALL WAVES FINISHED!!!");
            wave_state = -1; /* final state: show winner */
        }
    }

    if(wave_state == -1)
    {
        InitSystemCinematics();
        SDL_SetWindowTitle(window, "The Mutant's");
        PlayCinematic("cinematics/level2_on_the_way_to_the_plane.mp4", renderer);
        ShutdownCinematicsSystem();
        SDL_RenderClear(renderer);
        wave_state = -2; /* prevent replaying cinematic */
    }

    if(wave_state == -2)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        CleanupZombieSystem();
        CleanupPlayer();
        CleanupBackground();
        CleanupProjectileSystem();
        TTF_CloseFont(font_kills);
        CloseMusic();
        InitMusic();
        PlayMusicStateMenu();
        Init_State_Menu();
        Update_State_Menu();
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

    /* If both waves finished, render the winner image centered on screen */
    if (wave_state == -1 && winner_tex) {
        int w = 0, h = 0;
        SDL_QueryTexture(winner_tex, NULL, NULL, &w, &h);
        if (w > 0 && h > 0) {
            SDL_Rect dest = { backgroundSrcRect.w/2 - w/2, backgroundSrcRect.h/2 - h/2, w, h };
            SDL_RenderCopy(renderer, winner_tex, NULL, &dest);
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

    if (winner_tex) {
        SDL_DestroyTexture(winner_tex);
        winner_tex = NULL;
    }
    if (tmp_surf_winner) {
        SDL_FreeSurface(tmp_surf_winner);
        tmp_surf_winner = NULL;
    }
    if(walk_zombie.tex_walk_zombie) {
        SDL_DestroyTexture(walk_zombie.tex_walk_zombie);
        walk_zombie.tex_walk_zombie = NULL;
    }
    if(idle_zombie.tex_zombie_idle) {
        SDL_DestroyTexture(idle_zombie.tex_zombie_idle);
        idle_zombie.tex_zombie_idle = NULL;
    }
}

/* kept for compatibility with existing calls */
void RenderZombieIdle()
{
    /* simple wrapper that renders the zombie at its current position */
    RenderZombies();
}
