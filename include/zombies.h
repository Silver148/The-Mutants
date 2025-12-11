#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <SDL2/SDL.h>
#include "anim_manager.h"

#define MAX_ZOMBIES 50

/* tuning */
#define ZOMBIE_WIDTH 64
#define ZOMBIE_HEIGHT 64
#define ZOMBIE_DETECT_RADIUS 180.0f

/* HITBOX VARIABLES FOR ZOMBIES */
#define ZOMBIE_HITBOX_OFFSET_X 24
#define ZOMBIE_HITBOX_OFFSET_Y 8
#define ZOMBIE_HITBOX_WIDTH (ZOMBIE_WIDTH - 48)
#define ZOMBIE_HITBOX_HEIGHT (ZOMBIE_HEIGHT - 24)

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
    int health;
    int alive; /* boolean-like flag */
    int id;
} ZOMBIE;

void LoadSpritesZombies();
void InitZombieSys();
int SpawnZombie(float x, float y);
void SpawnZombieRandom();
void CleanupZombieSystem();

/*RENDER FUCTIONS*/
void UpdateZombies();
void RenderZombies();
void RenderZombieIdle();
void DeleteZombies();


/*HITBOX FUNC*/
void ShowHitboxZombie(int zombie_index);

#endif