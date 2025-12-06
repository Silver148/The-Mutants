#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <SDL2/SDL.h>
#include "anim_manager.h"

typedef enum {
    IDLE_Z,
    WALK_Z
} ZombieState;

typedef struct {
    SDL_Texture* tex_zombie_idle;
    SDL_Surface* tmp_surf_zombie_idle;
    Animation idle_anim;
} IDLE_ZOMBIE;

typedef struct {
    SDL_Texture* tex_walk_zombie;
    SDL_Surface* tmp_surf_walk_zombie;
    Animation walk_anim;
} WALK_ZOMBIE;

typedef struct {
    float x, y;
    float base_y;
    float speed;
    int dir;
    float wander_timer;
    ZombieState state;
    SDL_Rect src;
    SDL_Rect dest;
    float attack_cooldown;
    float attack_timer;
    int attack_damage;
} ZOMBIE;

void LoadSpritesZombies();
void InitZombie(float x, float y);

/*RENDER FUCTIONS*/
void UpdateZombies();
void RenderZombies();
void RenderZombieIdle();

/*HITBOX FUNC*/
void ShowHitboxZombie();

#endif