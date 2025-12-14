/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: projectiles.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "projectiles.h"
#include "global_vars.h"
#include "player.h"
#include "zombies.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>

extern float deltaTime; /* from delta_time.c */
extern ZOMBIE zombies[MAX_ZOMBIES];/*zombies instance (from zombies.c) */
extern int num_zombies;; 

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
    
    for(int i = 0; i < MAX_PROJECTILES; i++) {
        if(!projectiles[i].active) continue;

        projectiles[i].x += projectiles[i].vx * deltaTime;
        projectiles[i].y += projectiles[i].vy * deltaTime;

        if(projectiles[i].x < -50 || projectiles[i].x > 700 || 
           projectiles[i].y < -50 || projectiles[i].y > 530) {
            projectiles[i].active = 0;
            continue;
        }

        SDL_Rect prect = { 
            (int)projectiles[i].x, 
            (int)projectiles[i].y, 
            projectiles[i].w, 
            projectiles[i].h 
        };
        
        SDL_Rect prect_hitbox = {
            (int)projectiles[i].x + PROJECTILE_HITBOX_OFFSET_X,
            (int)projectiles[i].y + PROJECTILE_HITBOX_OFFSET_Y,
            PROJECTILE_HITBOX_WIDTH,
            PROJECTILE_HITBOX_HEIGHT
        };

        int projectile_hit = 0;
        
        for(int z = 0; z < MAX_ZOMBIES && !projectile_hit; z++) {
            if(!zombies[z].alive) continue;
            
            //ZOMBIE HITBOX
            SDL_Rect zrect = {
                zombies[z].dest.x + ZOMBIE_HITBOX_OFFSET_X,
                zombies[z].dest.y + ZOMBIE_HITBOX_OFFSET_Y,
                ZOMBIE_HITBOX_WIDTH,
                ZOMBIE_HITBOX_HEIGHT
            };
            
            //AAB Collision check
            if (prect_hitbox.x < zrect.x + zrect.w && 
                prect_hitbox.x + prect_hitbox.w > zrect.x &&
                prect_hitbox.y < zrect.y + zrect.h && 
                prect_hitbox.y + prect_hitbox.h > zrect.y) {
                
                // HIT
                zombies[z].health -= projectiles[i].damage;
                projectile_hit = 1;
                projectiles[i].active = 0;
                
                #ifdef DEBUG
                SDL_Log("Zombie hit! ID: %d, HP: %d\n", 
                       zombies[z].id, zombies[z].health);
                #endif
                
                if(zombies[z].health <= 0) {
                    zombies[z].alive = 0;
                    zombies[z].speed = 0.0f;
                    num_zombies--;
                    
                    #ifdef DEBUG
                    SDL_Log("Zombie %d eliminado! Zombies restantes: %d\n", 
                           zombies[z].id, num_zombies);
                    #else
                    SDL_Log("Zombie died\n");
                    #endif
                    
                }
                
                break;
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
