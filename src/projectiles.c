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
#include "states.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>

extern float deltaTime; /* from delta_time.c */
extern ZOMBIE zombies[MAX_ZOMBIES];/*zombies instance (from zombies.c) */
extern int num_zombies;; 
int counter_kills = 0;

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
            /* log also current camera so we can correlate world->screen coords */
            extern SDL_Rect backgroundSrcRect; /* from state_game.c */
            int screen_x = (int)x - backgroundSrcRect.x;
            int screen_y = (int)y - backgroundSrcRect.y;
            #ifdef DEBUG
            SDL_Log("SpawnProjectile: world=(%.1f,%.1f) bgSrc=(%d,%d) screen=(%d,%d) vx=%.1f vy=%.1f dmg=%d slot=%d",
                    x, y, backgroundSrcRect.x, backgroundSrcRect.y, screen_x, screen_y, vx, vy, damage, i);
            #endif
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

                    extern int backgroundImgW;
                    extern int backgroundImgH;
                    extern SDL_Rect backgroundRect; /* window dest rect from state_game.c */
                    /* If background image dimensions are not initialized yet, skip
                         out-of-bounds removal to avoid deactivating projectiles wrongly. */
                    if (backgroundImgW <= 0 || backgroundImgH <= 0) {
                        SDL_Log("UpdateProjectiles: skipping bounds check because backgroundImg=(%d,%d)", backgroundImgW, backgroundImgH);
                    } else {
                        /* Use the larger of the image size and the window destination size
                             as the world bounds so cropping doesn't incorrectly shorten limits. */
                        int worldW = (backgroundImgW > backgroundRect.w) ? backgroundImgW : backgroundRect.w;
                        int worldH = (backgroundImgH > backgroundRect.h) ? backgroundImgH : backgroundRect.h;
                        if(projectiles[i].x < -50 || projectiles[i].x > worldW + 50 ||
                             projectiles[i].y < -50 || projectiles[i].y > worldH + 50) {
                                projectiles[i].active = 0;
                                #ifdef DEBUG
                                SDL_Log("UpdateProjectiles: projectile[%d] deactivated (out of bounds) world=(%.1f,%.1f) worldBounds=(%d,%d)",
                                                i, projectiles[i].x, projectiles[i].y, worldW, worldH);
                                #endif
                                continue;
                        }
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
                SDL_Log("UpdateProjectiles: projectile[%d] hit zombie[%d] at world=(%.1f,%.1f) dmg=%d", i, z, projectiles[i].x, projectiles[i].y, projectiles[i].damage);
                
                #ifdef DEBUG
                SDL_Log("Zombie hit! ID: %d, HP: %d\n", 
                       zombies[z].id, zombies[z].health);
                #endif
                
                if(zombies[z].health <= 0) {
                      zombies[z].alive = 0;
                      zombies[z].speed = 0.0f;
                      num_zombies--;
                      /* increment global kills counter and update texture */
                      counter_kills++;
                      #ifdef DEBUG
                      SDL_Log("counter_kills incremented -> %d", counter_kills);
                      #endif
                      UpdateKillsTexture(counter_kills);
                    
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
        extern SDL_Rect backgroundSrcRect; /* declared in state_game.c */
        SDL_Rect r = { (int)projectiles[i].x - backgroundSrcRect.x, (int)projectiles[i].y - backgroundSrcRect.y, projectiles[i].w, projectiles[i].h };
        /* draw the small projectile rect */
        SDL_RenderFillRect(renderer, &r);
        
        #ifdef DEBUG
          /* Debug overlay: draw a larger visible red filled rectangle so bullets
              are impossible to miss. Use alpha blend to ensure visibility. */
          SDL_BlendMode oldMode;
          SDL_GetRenderDrawBlendMode(renderer, &oldMode);
          SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
          SDL_Rect dbgFill = { r.x - r.w, r.y - r.h, r.w * 3, r.h * 3 };
          SDL_RenderFillRect(renderer, &dbgFill);
          /* restore previous blend mode */
          SDL_SetRenderDrawBlendMode(renderer, oldMode);
        #endif
    }
}

void CleanupProjectileSystem()
{
    memset(projectiles, 0, sizeof(projectiles));
}
