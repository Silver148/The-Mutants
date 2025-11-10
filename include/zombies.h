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

/* Basic zombie entity used by the game */
typedef struct {
    float x;            /* world position X */
    float y;            /* world position Y */
    float base_y;       /* ground Y to avoid flying when player jumps */
    float speed;        /* movement speed (px/sec) */
    int dir;            /* -1 left, 0 none, 1 right */
    float wander_timer; /* countdown for random movement */
    StatesZombie state; /* animation/state */
    SDL_Rect src;       /* texture source rect */
    SDL_Rect dest;      /* texture destination rect */
    /* attack/collision */
    float attack_timer;     /* current cooldown timer (seconds) */
    float attack_cooldown;  /* seconds between attacks */
    int attack_damage;      /* damage applied per hit */
} ZOMBIE;

/* ZOMBIES API */
void LoadSpritesZombies();
void InitZombie(float x, float y);
void UpdateZombies();
void RenderZombies();

/* legacy helper kept for compatibility */
void RenderZombieIdle();

#endif