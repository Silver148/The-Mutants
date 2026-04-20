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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void RenderZombies(void);

/* camera source rect from state_game.c */
extern SDL_Rect backgroundSrcRect;
extern int backgroundImgW;
extern int backgroundImgH;
/* allow changing background from other modules */
extern void SetBackgroundImage(const char* path);
extern int counter_kills;

/* file-scope state so all functions can access it */
static int wave_state = 0;
/* Track current level number (1 = initial, 2 = second part, 3 = airport1, 4 = LAB1) */
int current_level = 1;
/* level3 phase: 0 = not in level3, 1 = first part (airport1), 2 = second part (airport2) */
static int level3_phase = 0;
static int phase2_kills = 0;
/* extra waves for level3 phase 2 */
static WAVE extra1, extra2, extra3, extra4, extra5;
static SDL_Surface* tmp_surf_winner = NULL;
static SDL_Texture* winner_tex = NULL;
/* Level passed screen */
#if 0
/* NOTE: SDL_image does not reliably provide animated GIF frame control across
    platforms; we implement a "show once" flag to ensure the level-passed
    asset (PNG or GIF) is displayed only a single time during the session. */
#endif
static SDL_Surface* tmp_surf_level_passed = NULL;
static SDL_Texture* level_passed_tex = NULL;
static float level_passed_timer = 0.0f;
static int level_passed_shown = 0; /* 0 = not shown yet, 1 = already shown */
static int level_passed_loaded_as_gif = 0;
/* Flag to indicate the current level was completed and should show pass screen */
static int level_completed_flag = 0;
#define LEVEL_PASSED_DURATION 3.0f
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

static WAVE wave1, wave2, wave3, wave4, wave5, wave6, wave7, wave8, wave9, wave10, wave11, wave12, wave13, wave14;

static int find_free_zombie_slot() {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].alive) {
            return i;
        }
    }
    return -1;
}

void StartLevel4(void)
{
    if(wave_state == 9) {
        SDL_Log("StartLevel4 called but wave_state is already 9, ignoring redundant call");
        return;
    }
    
    SDL_Log("Transitioning to Level 4 - loading background sprites/airport2.png");
    current_level = 4;
    SetBackgroundImage("sprites/airport2.png");
    health = MAX_HEALTH;
    SDL_Log("[zombies] Restored player health to %d at level 3 to 4 transition", health);
    for (int i = 0; i < MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
    num_zombies = 0;
    next_zombie_id = 0;
    InitWave(&wave7, 20, 2.0f, 5.0f, 0);
    InitWave(&wave8, 20, 2.0f, 5.0f, 0);
    wave7.timer = 0.0f;
    wave7.spawn_timer = 0.0f;
    wave7.spawn_active = true;
    SDL_Log("STARTING LEVEL 4 - WAVE 7 (wave_state set to 9)");
    SDL_Log("LEVEL 4 wave7 init: timer=%.2f spawn_timer=%.2f spawn_active=%d num_zombies=%d", wave7.timer, wave7.spawn_timer, wave7.spawn_active, wave7.num_zombies);
    fflush(stdout);
    wave_state = 9;
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

    /* load level-passed image: prefer PNG, fallback to GIF if present */
    tmp_surf_level_passed = IMG_Load("sprites/level_passed.png");
    level_passed_loaded_as_gif = 0;
    if (!tmp_surf_level_passed) {
        SDL_Log("sprites/level_passed.png not found, trying sprites/level_passed.gif");
        tmp_surf_level_passed = IMG_Load("sprites/level_passed.gif");
        if (tmp_surf_level_passed) level_passed_loaded_as_gif = 1;
    }

    if (tmp_surf_level_passed) {
        level_passed_tex = SDL_CreateTextureFromSurface(renderer, tmp_surf_level_passed);
        if (level_passed_tex) {
            SDL_Log("Loaded level_passed image (gif=%d)", level_passed_loaded_as_gif);
        } else {
            SDL_Log("Failed to create texture from level_passed surface");
        }
        SDL_FreeSurface(tmp_surf_level_passed);
        tmp_surf_level_passed = NULL;
    } else {
        level_passed_tex = NULL; /* optional: game will continue without it */
        SDL_Log("Level passed image not found: sprites/level_passed.png or .gif");
    }
}

static void get_random_spawn_position(float* x, float* y) {
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();

    int margin = 200; /* spawn within 200 pixels of player on X */
    int min_x = (int)(player_x - margin);
    int max_x = (int)(player_x + margin);
    if (min_x < 50) min_x = 50;
    if (max_x > backgroundImgW - 50) max_x = backgroundImgW - 50;
    if (max_x <= min_x) max_x = min_x + 1;

    do {
        *x = (float)(min_x + rand() % (max_x - min_x));
    } while (fabsf(*x - player_x) < 50); /* minimum 50 pixels from player */

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
        fflush(stdout);
    
    return z->id;
}

void SpawnZombieRandom() {
    float x, y;
    get_random_spawn_position(&x, &y);
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();
    SDL_Log("SpawnZombieRandom: player=(%.1f,%.1f) spawn=(%.1f,%.1f) world=(%d,%d)", player_x, player_y, x, y, backgroundImgW, backgroundImgH);
    fflush(stdout);
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
    SDL_Log("ENTER UpdateZombies: wave_state=%d current_level=%d num_zombies=%d", wave_state, current_level, num_zombies);
    fflush(stdout);
    float player_x = GetPositionPlayerX();
    float player_y = GetPositionPlayerY();

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
        InitWave(&wave1, 10, 2.0f, 2.0f, 0);
        wave_state = 1;
        SDL_Log("STARTING WAVE 1");
    }

    if (wave_state == 1)
    {
        int r = UpdateWave(&wave1, z);

        if (r == -1)
        {
            wave_state = 2;
            SDL_Log("WAVE 1 FINISHED");
        }
    }

    if(wave_state == 2)
    {
        InitWave(&wave2, 10, 2.0f, 3.0f, 0);

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
        if(z->base_y > backgroundImgH - ZOMBIE_HEIGHT) z->base_y = 355;
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
        
        if (SDL_HasIntersection(&zrect, &prect)) {
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
            if (level3_phase == 2) phase2_kills++;
        }
    }

    /* If waves finished earlier and we're waiting for all zombies to be cleared,
       perform the background change only once there are no active zombies. */
    if (pending_bg_change && num_zombies == 0) {
        /* Only perform this pending background change if we're still in the initial level
           (current_level == 1). If we've already moved to level 3 or another phase,
           ignore this pending flag to avoid reverting backgrounds unexpectedly. */
        pending_bg_change = 0; /* consume the flag regardless */
        if (current_level == 1) {
            current_level = 2;
            /* Skip the win1 cinematic; just change background and continue */
            SDL_Log("All zombies cleared after waves. Changing background now.");
            /* change background, then start two additional waves */
            SetBackgroundImage("sprites/113 sin título_20260104134157~2.png");
            /* Restore player health at level transition */
            health = MAX_HEALTH;
            SDL_Log("[zombies] Restored player health to %d at level 1 to 2 transition", health);
              /* start wave 3 (level 2 begins). We'll chain additional waves so level 2
                  has double the number of waves compared to the initial part. */
              InitWave(&wave3, 10, 2.0f, 3.0f, 0);
              for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
              SDL_Log("STARTING WAVE 3");
              wave_state = 5; /* wave3 running */
        } else {
            SDL_Log("Ignored pending_bg_change because current_level=%d", current_level);
        }
    }

    /* Handle level 3 phase 2 start after initial waves */
    if (wave_state == 4 && current_level == 3 && level3_phase < 2) {
        SDL_Log("LEVEL 3: completed initial waves -> starting phase 2 (airport2)");
        level3_phase = 2;
        phase2_kills = 0;
        SDL_Log("LEVEL 3: loading background sprites/airport2.png");
        SetBackgroundImage("sprites/airport2.png");
        for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
        num_zombies = 0;
        next_zombie_id = 0;
        InitWave(&extra1, 12, 3.0f, 4.0f, 0);
        SDL_Log("STARTING LEVEL3 PHASE2 - EXTRA WAVE 1");
        wave_state = 20;
    }

    /* Handle level 4 start if switched during level 3 waves */
    if (wave_state == 4 && current_level == 4) {
        StartLevel4();
        wave_state = 9;
    }

    /* Additional waves logic (wave3 -> wave4 -> final) */
    if (wave_state == 5) {
        int r = UpdateWave(&wave3, z);
        if (r == -1) {
            SDL_Log("WAVE 3 FINISHED");
            /* init wave4 */
            InitWave(&wave4, 10, 2.0f, 3.0f, 0);
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
            SDL_Log("STARTING WAVE 4");
            wave_state = 6; /* wave4 running */
        }
    }

    if (wave_state == 6) {
        int r = UpdateWave(&wave4, z);
        if (r == -1) {
            SDL_Log("WAVE 4 FINISHED");
            /* init wave5 (extra for level 2) */
            InitWave(&wave5, 10, 2.0f, 3.0f, 0);
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
            SDL_Log("STARTING WAVE 5");
            wave_state = 7; /* wave5 running */
        }
    }

    if (wave_state == 7) {
        int r = UpdateWave(&wave5, z);
        if (r == -1) {
            SDL_Log("WAVE 5 FINISHED");
            if (current_level == 4) {
                StartLevel4();
            } else if (current_level == 3 && level3_phase < 2) {
                SDL_Log("LEVEL 3: completed 5 waves -> starting phase 2 (airport2)");
                /* change background to airport2 and start 5 additional waves */
                level3_phase = 2;
                phase2_kills = 0;
                SDL_Log("LEVEL 3: loading background sprites/airport2.png");
                SetBackgroundImage("sprites/airport2.png");
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
                next_zombie_id = 0;
                InitWave(&extra1, 12, 3.0f, 4.0f, 0);
                SDL_Log("STARTING LEVEL 3 PHASE2 - EXTRA WAVE 1");
                wave_state = 20; /* start extra wave chain */
            } else {
                InitWave(&wave6, 10, 2.0f, 3.0f, 0);
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
                SDL_Log("STARTING WAVE 6");
                wave_state = 8; /* wave6 running */
            }
        }
    }

    if (wave_state == 8) {
        int r = UpdateWave(&wave6, NULL);
        if (r == -1) {
            if (current_level == 4) {
                StartLevel4();
                return;
            } else {
                SDL_Log("WAVE 6 FINISHED");
                SDL_Log("ALL WAVES FINISHED!!!");
                /* Transition to Level 3: change background and start new waves */
                current_level = 3;
                SDL_Log("Transitioning to Level 3 - loading background sprites/airport1.png");
                SetBackgroundImage("sprites/airport1.png");
                /* Restore player health at level transition */
                health = MAX_HEALTH;
                SDL_Log("[zombies] Restored player health to %d at level 2 to 3 transition", health);
                /* Ensure pending_bg_change is cleared so previous level triggers don't run */
                pending_bg_change = 0;
                /* Reset zombies and start two new waves for level 3 */
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
                num_zombies = 0;
                next_zombie_id = 0;
                InitWave(&wave1, 12, 3.0f, 4.0f, 0);
            InitWave(&wave2, 12, 3.0f, 4.0f, 0);
            SDL_Log("STARTING LEVEL 3 - WAVE 1");
            wave_state = 1; /* start wave1 for level 3 */
            }
        }
    }

    /* Level 4 waves: wave7 and wave8 */
    SDL_Log("About to check wave_state ==9, wave_state=%d", wave_state);
    if (wave_state == 9) {
        SDL_Log("LEVEL 4: wave_state 9 branch entered");
        SDL_Log("Wave state is 9, calling UpdateWave");
        int r = UpdateWave(&wave7, NULL);
        SDL_Log("LEVEL 4: UpdateWave returned %d", r);
        if (r == -1) {
            SDL_Log("LEVEL 4: WAVE 7 FINISHED");
            InitWave(&wave8, 20, 1.0f, 2.0f, 0);
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
            SDL_Log("STARTING LEVEL 4 - WAVE 8");
            wave_state = 10; /* wave8 running */
        }
    }

    if (wave_state == 10) {
        int r = UpdateWave(&wave8, NULL);
        if (r == -1) {
            SDL_Log("LEVEL 4: WAVE 8 FINISHED - Transitioning to Level 5");
            current_level = 5;
            SDL_Log("Transitioning to Level 5 - loading background sprites/LAB1.png");
            SetBackgroundImage("sprites/LAB1.png");
            /* Restore player health at level transition */
            health = MAX_HEALTH;
            SDL_Log("[zombies] Restored player health to %d at level 4 to 5 transition", health);
            /* Reset zombies and start two new waves for level 5 */
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
            num_zombies = 0;
            next_zombie_id = 0;
            InitWave(&wave9, 18, 3.0f, 4.0f, 20.0f);
            InitWave(&wave10, 18, 3.0f, 4.0f, 22.0f);
            SDL_Log("STARTING LEVEL 5 - WAVE 9");
            wave_state = 11; /* wave9 running */
        }
    }

    /* Level 5 waves: wave9 and wave10 */
    if (wave_state == 11) {
        int r = UpdateWave(&wave9, z);
        if (r == -1) {
            SDL_Log("LEVEL 5: WAVE 9 FINISHED");
            SDL_Log("STARTING LEVEL 5 - WAVE 10");
            wave_state = 12; /* wave10 running */
        }
    }

    if (wave_state == 12) {
        int r = UpdateWave(&wave10, z);
        if (r == -1) {
            SDL_Log("LEVEL 5: WAVE 10 FINISHED - Transitioning to Level 6");
            current_level = 6;
            SDL_Log("Transitioning to Level 6 - loading background sprites/airport1.png");
            SetBackgroundImage("sprites/airport1.png");
            /* Restore player health at level transition */
            health = MAX_HEALTH;
            SDL_Log("[zombies] Restored player health to %d at level 5 to 6 transition", health);
            /* Reset zombies and start four waves for level 6 */
            for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; zombies[i].alive = 0; }
            num_zombies = 0;
            next_zombie_id = 0;
            InitWave(&wave11, 20, 3.0f, 4.0f, 25.0f);
            InitWave(&wave12, 20, 3.0f, 4.0f, 27.0f);
            InitWave(&wave13, 20, 3.0f, 4.0f, 29.0f);
            InitWave(&wave14, 20, 3.0f, 4.0f, 31.0f);
            SDL_Log("STARTING LEVEL 6 - WAVE 11");
            wave_state = 13; /* wave11 running */
        }
    }

    /* Level 6 waves: wave11, wave12, wave13, wave14 */
    if (wave_state == 13) {
        int r = UpdateWave(&wave11, z);
        if (r == -1) {
            SDL_Log("LEVEL 6: WAVE 11 FINISHED");
            SDL_Log("STARTING LEVEL 6 - WAVE 12");
            wave_state = 14; /* wave12 running */
        }
    }

    if (wave_state == 14) {
        int r = UpdateWave(&wave12, z);
        if (r == -1) {
            SDL_Log("LEVEL 6: WAVE 12 FINISHED");
            SDL_Log("STARTING LEVEL 6 - WAVE 13");
            wave_state = 15; /* wave13 running */
        }
    }

    if (wave_state == 15) {
        int r = UpdateWave(&wave13, z);
        if (r == -1) {
            SDL_Log("LEVEL 6: WAVE 13 FINISHED");
            SDL_Log("STARTING LEVEL 6 - WAVE 14");
            wave_state = 16; /* wave14 running */
        }
    }

    if (wave_state == 16) {
        int r = UpdateWave(&wave14, z);
        if (r == -1) {
            SDL_Log("LEVEL 6: WAVE 14 FINISHED - Game completed");
            level_completed_flag = 1;
            health = MAX_HEALTH;
            SDL_Log("[zombies] Restored player health to %d due to game completion", health);
            wave_state = -1;
        }
    }

    /* Extra waves for level3 phase 2: states 20..24 */
    if (wave_state == 20) {
        SDL_Log("LEVEL3 PHASE2: Entering wave_state 20 branch");
        SDL_Log("Calling UpdateWave for extra1");
        int r = UpdateWave(&extra1, z);
        if (r == -1) {
            SDL_Log("LEVEL3 PHASE2: EXTRA WAVE 1 FINISHED");
            if (current_level == 4) {
                StartLevel4();
            } else {
                InitWave(&extra2, 12, 3.0f, 4.0f, 0);
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
                SDL_Log("STARTING LEVEL3 PHASE2 - EXTRA WAVE 2");
                wave_state = 21;
            }
        }
    }

    if (wave_state == 21) {
        int r = UpdateWave(&extra2, z);
        if (r == -1) {
            SDL_Log("LEVEL3 PHASE2: EXTRA WAVE 2 FINISHED");
            if (current_level == 4) {
                StartLevel4();
            } else {
                InitWave(&extra3, 12, 3.0f, 4.0f, 0);
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
                SDL_Log("STARTING LEVEL3 PHASE2 - EXTRA WAVE 3");
                wave_state = 22;
            }
        }
    }

    if (wave_state == 22) {
        int r = UpdateWave(&extra3, z);
        if (r == -1) {
            SDL_Log("LEVEL3 PHASE2: EXTRA WAVE 3 FINISHED");
            if (current_level == 4) {
                StartLevel4();
            } else {
                InitWave(&extra4, 12, 3.0f, 4.0f, 0);
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
                SDL_Log("STARTING LEVEL3 PHASE2 - EXTRA WAVE 4");
                wave_state = 23;
            }
        }
    }

    if (wave_state == 23) {
        int r = UpdateWave(&extra4, z);
        if (r == -1) {
            SDL_Log("LEVEL3 PHASE2: EXTRA WAVE 4 FINISHED");
            if (current_level == 4) {
                StartLevel4();
            } else {
                InitWave(&extra5, 12, 3.0f, 4.0f, 0);
                for(int i = 0; i<MAX_ZOMBIES; i++) { zombies[i].id = 0; next_zombie_id = 0; }
                SDL_Log("STARTING LEVEL3 PHASE2 - EXTRA WAVE 5");
                wave_state = 24;
            }
        }
    }

    if (wave_state == 24) {
        int r = UpdateWave(&extra5, z);
        if (r == -1) {
            SDL_Log("LEVEL3 PHASE2: EXTRA WAVE 5 FINISHED - level 3 completed");
            StartLevel4();
        }
    }

    if(wave_state == -1)
    {
        /* Only show the level-passed screen when the level_completed_flag
           was set at the moment the full level finished. Otherwise skip. */
        if (level_completed_flag && !level_passed_shown) {
            level_passed_timer = 0.0f;
            SetBackgroundImage("sprites/113 sin título_20260104134157~2.png");
            wave_state = -3;
        } else {
            /* proceed directly to cleanup/menu (skip level2 cinematic) */
            SDL_RenderClear(renderer);
            wave_state = -2; /* prevent replaying cinematic */
        }
        /* reset the completion flag so next level must set it again */
        level_completed_flag = 0;
    }

    if (wave_state == -3) {
        /* Wait LEVEL_PASSED_DURATION seconds showing the pass screen, then play cinematic */
        level_passed_timer += deltaTime;
        SDL_Log("Level-passed timer: %f (threshold %f)", level_passed_timer, LEVEL_PASSED_DURATION);
        if (level_passed_timer >= LEVEL_PASSED_DURATION) {
            /* mark as shown so future level completions don't replay it */
            level_passed_shown = 1;
            /* Prefer cinematics/level_passed.mp4 (user path), fallback to sprites/ */
            FILE* f = fopen("cinematics/level_passed.mp4", "rb");
            if (f) {
                fclose(f);
                InitSystemCinematics();
                SDL_SetWindowTitle(window, "The Mutant's");
                PlayCinematic("cinematics/level_passed.mp4", renderer);
                ShutdownCinematicsSystem();
                SDL_RenderClear(renderer);
            } else {
                FILE* f2 = fopen("sprites/level_passed.mp4", "rb");
                if (f2) {
                    fclose(f2);
                    InitSystemCinematics();
                    SDL_SetWindowTitle(window, "The Mutant's");
                    PlayCinematic("sprites/level_passed.mp4", renderer);
                    ShutdownCinematicsSystem();
                    SDL_RenderClear(renderer);
                } else {
                    /* no cinematic available, just clear and continue */
                    SDL_RenderClear(renderer);
                }
            }
            wave_state = -2; /* prevent replaying cinematic */
        }
    }

    if(wave_state == -2)
    {   
        /* CLEAR STATE GAME RESOURCES*/                          
        CleanupBackground();
        CleanupZombieSystem();
        CleanupPlayer();
        CleanupProjectileSystem();
        CleanupKillsTexture();
        CleanupAmmunitions();

        /* STOP MUSIC*/
        CloseMusic();
        /* INIT MENU MUSIC*/
        InitMusic();
        PlayMusicStateMenu();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_ShowCursor(SDL_ENABLE);
        SDL_SetRelativeMouseMode(SDL_FALSE);

        /* CLEAR SCREN*/
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        /*INIT STATE MENU :)*/
        Init_State_Menu();
        Update_State_Menu();
    }
}


void RenderZombies(void) 
{
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

    /* If level passed display state (-3), render the pass image fullscreen */
    if (wave_state == -3) {
        SDL_Log("RenderZombies: wave_state == -3, level_passed_tex=%p", (void*)level_passed_tex);
        if (level_passed_tex) {
        int ww = 0, hh = 0;
        if (window) SDL_GetWindowSize(window, &ww, &hh);
        if (ww == 0 || hh == 0) SDL_GetRendererOutputSize(renderer, &ww, &hh);
        if (ww > 0 && hh > 0) {
            SDL_Rect dest = {0, 0, ww, hh};
            SDL_RenderCopy(renderer, level_passed_tex, NULL, &dest);
        }
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
void RenderZombieIdle()
{
    /* simple wrapper that renders the zombie at its current position */
    RenderZombies();
}
void CleanupZombieSystem() {
 

}
