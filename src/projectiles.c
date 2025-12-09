#include "projectiles.h"
#include "global_vars.h"
#include "player.h"
#include "zombies.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>

extern float deltaTime; /* from delta_time.c */
extern ZOMBIE zombie; /* single zombie instance (from zombies.c) */

static Projectile projectiles[MAX_PROJECTILES];

void InitProjectiles()
{
    memset(projectiles, 0, sizeof(projectiles));
}

void SpawnProjectile(float x, float y, float vx, float vy, int damage)
{
    for(int i=0;i<MAX_PROJECTILES;i++){
        if(!projectiles[i].active){
            projectiles[i].active = 1;
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].vx = vx;
            projectiles[i].vy = vy;
            projectiles[i].w = 12;
            projectiles[i].h = 6;
            projectiles[i].damage = damage;
            return;
        }
    }
}

void UpdateProjectiles()
{
    extern ZOMBIE zombie; /* from zombies.c */
    for(int i=0;i<MAX_PROJECTILES;i++){
        if(!projectiles[i].active) continue;

        projectiles[i].x += projectiles[i].vx * deltaTime;
        projectiles[i].y += projectiles[i].vy * deltaTime;

        /* off-screen kill */
        if(projectiles[i].x < -50 || projectiles[i].x > 700 || projectiles[i].y < -50 || projectiles[i].y > 530){
            projectiles[i].active = 0;
            continue;
        }

        /* collision with single zombie (AABB) */
        if(zombie.alive){
            SDL_Rect prect = { (int)projectiles[i].x, (int)projectiles[i].y, projectiles[i].w, projectiles[i].h };
            SDL_Rect zrect = { zombie.dest.x, zombie.dest.y, zombie.dest.w, zombie.dest.h };
            if (prect.x < zrect.x + zrect.w && prect.x + prect.w > zrect.x &&
                prect.y < zrect.y + zrect.h && prect.y + prect.h > zrect.y)
            {
                /* hit */
                zombie.health -= projectiles[i].damage;
                projectiles[i].active = 0;
                SDL_Log("Zombie hit! health=%d\n", zombie.health);
                if(zombie.health <= 0){
                    zombie.alive = 0;
                    zombie.speed = 0.0f;
                    DeleteZombies();
                    SDL_Log("Zombie died\n");
                }
            }
        }
    }
}

void RenderProjectiles()
{
    SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);
    for(int i=0;i<MAX_PROJECTILES;i++){
        if(!projectiles[i].active) continue;
        SDL_Rect r = { (int)projectiles[i].x, (int)projectiles[i].y, projectiles[i].w, projectiles[i].h };
        SDL_RenderFillRect(renderer, &r);
    }
}
