#ifndef PROJECTILES_H
#define PROJECTILES_H

#include <SDL2/SDL.h>

#define MAX_PROJECTILES 32

typedef struct {
    int active;
    float x;
    float y;
    float vx;
    float vy;
    int w;
    int h;
    int damage;
} Projectile;

/* API */
void InitProjectiles();
void SpawnProjectile(float x, float y, float vx, float vy, int damage);
void UpdateProjectiles();
void RenderProjectiles();


#endif // PROJECTILES_H
