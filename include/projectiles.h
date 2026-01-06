#ifndef PROJECTILES_H
#define PROJECTILES_H

#include <SDL2/SDL.h>

#define MAX_PROJECTILES 32
#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_DAMAGE 25

#define PROJECTILE_HITBOX_OFFSET_X 2
#define PROJECTILE_HITBOX_OFFSET_Y 2
#define PROJECTILE_HITBOX_WIDTH 12
#define PROJECTILE_HITBOX_HEIGHT 12

typedef struct {
    float x, y;
    float vx, vy;
    int w, h;
    int damage;
    int active;
    Uint32 spawn_time;
    SDL_Texture* texture;
    SDL_Rect src_rect;
} Projectile;

/* API */
void InitProjectiles();
void SpawnProjectile(float x, float y, float vx, float vy, int damage);
void UpdateProjectiles();
void RenderProjectiles();
void CleanupProjectileSystem();


#endif // PROJECTILES_H
