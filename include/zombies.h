#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <SDL2/SDL.h>
#include "anim_manager.h"

#define MAX_ZOMBIES 512

/* tuning */
#define ZOMBIE_WIDTH 64
#define ZOMBIE_HEIGHT 64
#define ZOMBIE_DETECT_RADIUS 180.0f

/* HITBOX VARIABLES FOR ZOMBIES (reduced) */
/* centered smaller hitbox: width = 12, height = 30 for ZOMBIE_WIDTH/HEIGHT = 64 */
#define ZOMBIE_HITBOX_WIDTH (ZOMBIE_WIDTH - 52)   /* 64 - 52 = 12 */
#define ZOMBIE_HITBOX_HEIGHT (ZOMBIE_HEIGHT - 34) /* 64 - 34 = 30 */
#define ZOMBIE_HITBOX_OFFSET_X ((ZOMBIE_WIDTH - ZOMBIE_HITBOX_WIDTH) / 2)
#define ZOMBIE_HITBOX_OFFSET_Y ((ZOMBIE_HEIGHT - ZOMBIE_HITBOX_HEIGHT) / 2)

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