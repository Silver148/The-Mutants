/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: player.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "player.h"
#include "delta_time.h"
#include "global_vars.h"
#include "anim_manager.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

extern SDL_Renderer* renderer;
/* camera source rect from state_game.c */
extern SDL_Rect backgroundSrcRect;
extern int backgroundImgW;
extern int backgroundImgH;
extern int worldBarrierX;
extern int worldBarrierLeftX;

#define POS_Y 350
#define POS_X 100

/*IDLE PLAYER*/
IDLE_PLAYER idle_player;

/*WALK PLAYER*/
WALK_PLAYER walk_player;

/*JUMP PLAYER*/
JUMP_PLAYER jump_player;

/*SHOOT PLAYER*/
SHOOT_PLAYER shoot_player;
/*SHOOT WALK PLAYER*/
SHOOT_WALK_PLAYER shoot_walk_player;

#define SHOOT_FRAMES 4

/* shoot animation playing flag and previous state */
static bool shoot_anim_playing = false;
static bool shoot_looping = false; /* true while Z is held and we should loop the anim */
static StatesPlayer shoot_prev_state = IDLE;
static int shoot_anim_type = 0; /* 0 = normal shoot, 1 = walk-shoot */

void CleanTextureShoot(void);

/* Query helper for other modules to know if shoot animation is playing */
bool IsPlayerShooting(void)
{
    return shoot_anim_playing;
}

/* Start continuous shoot animation (loop while held) */
extern StatesPlayer states_player; /* forward declaration to satisfy functions above */
void StartPlayerShooting(void)
{
    if (!shoot_player.tex_shootplayer) return;
    if (shoot_anim_playing) { shoot_looping = true; return; }
    shoot_anim_type = 0;
    shoot_prev_state = states_player;
    states_player = SHOOT;
    Animation* anim = &shoot_player.shoot_anim;
    anim->current_frame = 0;
    anim->last_update_time = SDL_GetTicks();
    anim->src_rect.x = 0;
    anim->src_rect.y = 0;
    anim->src_rect.w = anim->frame_width;
    anim->src_rect.h = anim->frame_height;
    shoot_anim_playing = true;
    shoot_looping = true;
}

/* Start continuous shoot-walking animation (loop while held) */
void StartPlayerShootingWalk(void)
{
    if (!shoot_walk_player.tex_shootwalkplayer) {
        /* fallback to normal shoot if walk-shoot not available */
        StartPlayerShooting();
        return;
    }
    if (shoot_anim_playing) { shoot_looping = true; return; }
    shoot_anim_type = 1;
    shoot_prev_state = states_player;
    states_player = SHOOT; /* state is SHOOT, but anim type indicates walk variant */
    Animation* anim = &shoot_walk_player.shootwalk_anim;
    anim->current_frame = 0;
    anim->last_update_time = SDL_GetTicks();
    anim->src_rect.x = 0;
    anim->src_rect.y = 0;
    anim->src_rect.w = anim->frame_width;
    anim->src_rect.h = anim->frame_height;
    shoot_anim_playing = true;
    shoot_looping = true;
}

/* Stop continuous shoot animation and restore previous state */
void StopPlayerShooting(void)
{
    if (!shoot_anim_playing) return;
    shoot_looping = false;
    shoot_anim_playing = false;
    states_player = shoot_prev_state;
    /* reset animation frame */
    if (shoot_anim_type == 1) {
        shoot_walk_player.shootwalk_anim.current_frame = 0;
        shoot_walk_player.shootwalk_anim.src_rect.x = 0;
    } else {
        shoot_player.shoot_anim.current_frame = 0;
        shoot_player.shoot_anim.src_rect.x = 0;
    }
    shoot_anim_type = 0;
}


/*POSITION X AND Y OF THE PLAYER*/
float position_x = POS_X;
float position_y = POS_Y;

int last_position_x = 0;    
int last_position_y = 0;

int health = 170; 

/*STATES OF THE PLAYER*/
StatesPlayer states_player;

/* control de velocidad(boost) */
void SetPlayerSpeedMultiplier(float multiplier);
void ResetPlayerSpeed(void);

/*PLAYER SPEED*/
/* Base speeds: keep an original default base and a skin multiplier so boosts multiply both */
static float default_base_player_speed = 70.0f; /* original unmodified base */
static float base_player_speed = 70.0f; /* = default_base_player_speed * skin_multiplier */
static float skin_speed_multiplier = 1.0f; /* multiplier applied when skin changes */
float player_speed = 70.0f;
static bool skin_is_metal = false; /* true if current skin is metal/fast-type */

/*JUMP VARIABLES*/
bool is_jumping = false;
float jump_force = 250.0f;
float gravity = 900.0f;
float velocity_y = 0.0f;

extern float deltaTime;

extern int current_frame;
extern Uint32 last_update_time;

/*HITBOX VARIABLES*/
static Hitbox player_hitbox;
#define PLAYER_HITBOX_OFFSET_X 15
#define PLAYER_HITBOX_OFFSET_Y 10 
#define PLAYER_HITBOX_WIDTH (PLAYER_WIDTH - 30)
#define PLAYER_HITBOX_HEIGHT (PLAYER_HEIGHT - 20)

void UpdatePlayerHitbox(void) {

    player_hitbox.x = position_x + PLAYER_HITBOX_OFFSET_X;
    player_hitbox.y = position_y + PLAYER_HITBOX_OFFSET_Y;
    player_hitbox.w = PLAYER_HITBOX_WIDTH;
    player_hitbox.h = PLAYER_HITBOX_HEIGHT;
}

Hitbox GetPlayerHitbox(void) {
    UpdatePlayerHitbox();
    return player_hitbox;
}

void LoadSpritesPlayer() {   
    /* Only load default sprites if textures are not already set (preserve skins set from menu) */
    if (!idle_player.tex_idleplayer) {
        idle_player.tmp_surf_idleplayer = IMG_Load("sprites/idle_player_spritesheet.png");
        if (idle_player.tmp_surf_idleplayer) {
            idle_player.tex_idleplayer = SDL_CreateTextureFromSurface(renderer, idle_player.tmp_surf_idleplayer);
            SDL_FreeSurface(idle_player.tmp_surf_idleplayer);
            idle_player.tmp_surf_idleplayer = NULL;
            Animation_Init(&idle_player.idle_anim, PLAYER_WIDTH, PLAYER_HEIGHT, IDLE_FRAMES, FRAME_DURATION_PLAYER);
        } else {
            SDL_Log("LoadSpritesPlayer: failed to load default idle sprite: %s\n", "sprites/idle_player_spritesheet.png");
        }
    }

    if (!walk_player.tex_walkplayer) {
        walk_player.tmp_surf_walkplayer = IMG_Load("sprites/walk_player_spritesheet.png");
        if (walk_player.tmp_surf_walkplayer) {
            walk_player.tex_walkplayer = SDL_CreateTextureFromSurface(renderer, walk_player.tmp_surf_walkplayer);
            SDL_FreeSurface(walk_player.tmp_surf_walkplayer);
            walk_player.tmp_surf_walkplayer = NULL;
            Animation_Init(&walk_player.walk_anim, PLAYER_WIDTH, PLAYER_HEIGHT, WALK_FRAMES, FRAME_DURATION_PLAYER);
        } else {
            SDL_Log("LoadSpritesPlayer: failed to load default walk sprite: %s\n", "sprites/walk_player_spritesheet.png");
        }
    }

    if (!jump_player.tex_jumpplayer) {
        jump_player.tmp_surf_jumpplayer = IMG_Load("sprites/jump_player-spritesheet.png");
        if (jump_player.tmp_surf_jumpplayer) {
            jump_player.tex_jumpplayer = SDL_CreateTextureFromSurface(renderer, jump_player.tmp_surf_jumpplayer);
            SDL_FreeSurface(jump_player.tmp_surf_jumpplayer);
            jump_player.tmp_surf_jumpplayer = NULL;
            Animation_Init(&jump_player.jump_anim, PLAYER_WIDTH, PLAYER_HEIGHT, JUMP_FRAMES, FRAME_DURATION_PLAYER);
        } else {
            SDL_Log("LoadSpritesPlayer: failed to load default jump sprite: %s\n", "sprites/jump_player-spritesheet.png");
        }
    }

    if(!shoot_player.tex_shootplayer)
    {
        shoot_player.tmp_surf_shootplayer = IMG_Load("sprites/BANG_dessert-spritesheet.png");

        if(shoot_player.tmp_surf_shootplayer)
        {
            shoot_player.tex_shootplayer = SDL_CreateTextureFromSurface(renderer, shoot_player.tmp_surf_shootplayer);
            SDL_FreeSurface(shoot_player.tmp_surf_shootplayer);
            shoot_player.tmp_surf_shootplayer = NULL;
            Animation_Init(&shoot_player.shoot_anim, PLAYER_WIDTH, PLAYER_HEIGHT, SHOOT_FRAMES, FRAME_DURATION_PLAYER);
        }else{
            SDL_Log("LoadSpritesPlayer: failed to load default shoot sprite: %s\n", "sprites/BANG_dessert-spritesheet.png");
        }
    }

    /* shoot spritesheet not auto-loaded by default; use ChangePlayerShootSkin(path) to set it */
    /* shoot-walk spritesheet not auto-loaded by default; use ChangePlayerShootWalkSkin(path) to set it */

}

/* Replace current player sprites with provided file paths. Frees old textures. */
void ChangePlayerSkin(const char* idle_path, const char* walk_path, const char* jump_path)
{
    if(idle_path && *idle_path){
        if(idle_player.tex_idleplayer){ SDL_DestroyTexture(idle_player.tex_idleplayer); idle_player.tex_idleplayer = NULL; }
        if(idle_player.tmp_surf_idleplayer) { SDL_FreeSurface(idle_player.tmp_surf_idleplayer); idle_player.tmp_surf_idleplayer = NULL; }
        idle_player.tmp_surf_idleplayer = IMG_Load(idle_path);
        if(idle_player.tmp_surf_idleplayer){
            idle_player.tex_idleplayer = SDL_CreateTextureFromSurface(renderer, idle_player.tmp_surf_idleplayer);
            SDL_FreeSurface(idle_player.tmp_surf_idleplayer);
            idle_player.tmp_surf_idleplayer = NULL;
            Animation_Init(&idle_player.idle_anim, PLAYER_WIDTH, PLAYER_HEIGHT, IDLE_FRAMES, FRAME_DURATION_PLAYER);
            SDL_Log("ChangePlayerSkin: loaded idle skin '%s'\n", idle_path);
        } else {
            SDL_Log("Failed to load idle skin: %s\n", idle_path);
        }
    }

    if(walk_path && *walk_path){
        if(walk_player.tex_walkplayer){ SDL_DestroyTexture(walk_player.tex_walkplayer); walk_player.tex_walkplayer = NULL; }
        if(walk_player.tmp_surf_walkplayer) { SDL_FreeSurface(walk_player.tmp_surf_walkplayer); walk_player.tmp_surf_walkplayer = NULL; }
        walk_player.tmp_surf_walkplayer = IMG_Load(walk_path);
        if(walk_player.tmp_surf_walkplayer){
            walk_player.tex_walkplayer = SDL_CreateTextureFromSurface(renderer, walk_player.tmp_surf_walkplayer);
            SDL_FreeSurface(walk_player.tmp_surf_walkplayer);
            walk_player.tmp_surf_walkplayer = NULL;
            Animation_Init(&walk_player.walk_anim, PLAYER_WIDTH, PLAYER_HEIGHT, WALK_FRAMES, FRAME_DURATION_PLAYER);
            SDL_Log("ChangePlayerSkin: loaded walk skin '%s'\n", walk_path);
        } else {
            SDL_Log("Failed to load walk skin: %s\n", walk_path);
        }
    }

    if(jump_path && *jump_path){
        if(jump_player.tex_jumpplayer){ SDL_DestroyTexture(jump_player.tex_jumpplayer); jump_player.tex_jumpplayer = NULL; }
        if(jump_player.tmp_surf_jumpplayer) { SDL_FreeSurface(jump_player.tmp_surf_jumpplayer); jump_player.tmp_surf_jumpplayer = NULL; }
        jump_player.tmp_surf_jumpplayer = IMG_Load(jump_path);
        if(jump_player.tmp_surf_jumpplayer){
            jump_player.tex_jumpplayer = SDL_CreateTextureFromSurface(renderer, jump_player.tmp_surf_jumpplayer);
            SDL_FreeSurface(jump_player.tmp_surf_jumpplayer);
            jump_player.tmp_surf_jumpplayer = NULL;
            Animation_Init(&jump_player.jump_anim, PLAYER_WIDTH, PLAYER_HEIGHT, JUMP_FRAMES, FRAME_DURATION_PLAYER);
            SDL_Log("ChangePlayerSkin: loaded jump skin '%s'\n", jump_path);
        } else {
            SDL_Log("Failed to load jump skin: %s\n", jump_path);
        }
    }

    /* Adapt animation to the actual texture layout: compute frames from texture width.
       If idle has a detected frame size, reuse that division for walk/jump to keep consistent frames. */
    int idle_frame_w = 0, idle_frame_h = 0;
    if (idle_player.tex_idleplayer) {
        int w=0,h=0; SDL_QueryTexture(idle_player.tex_idleplayer, NULL, NULL, &w, &h);
        int detected_frames = (PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : 1;
        if (detected_frames <= 0) detected_frames = 1;
        int frame_w = (w / detected_frames);
        int frame_h = h;
        Animation_Init(&idle_player.idle_anim, frame_w, frame_h, detected_frames, FRAME_DURATION_PLAYER);
        idle_frame_w = frame_w; idle_frame_h = frame_h;
        SDL_Log("ChangePlayerSkin: idle texture %dx%d -> frames=%d frame_size=%dx%d\n", w, h, detected_frames, frame_w, frame_h);
        if (frame_w != PLAYER_WIDTH || frame_h != PLAYER_HEIGHT) {
            SDL_Log("Note: idle frame size differs from PLAYER_WIDTH/HEIGHT (%d x %d)\n", PLAYER_WIDTH, PLAYER_HEIGHT);
        }
    }

    if (walk_player.tex_walkplayer) {
        int w=0,h=0; SDL_QueryTexture(walk_player.tex_walkplayer, NULL, NULL, &w, &h);
        int frame_w = idle_frame_w ? idle_frame_w : ((PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : w);
        if (frame_w <= 0) frame_w = w;
        int detected_frames = (frame_w > 0) ? (w / frame_w) : 1;
        if (detected_frames <= 0) detected_frames = 1;
        int frame_h = idle_frame_h ? idle_frame_h : h;
        Animation_Init(&walk_player.walk_anim, frame_w, frame_h, detected_frames, FRAME_DURATION_PLAYER);
        SDL_Log("ChangePlayerSkin: walk texture %dx%d -> frames=%d frame_size=%dx%d\n", w, h, detected_frames, frame_w, frame_h);
        if (frame_w != PLAYER_WIDTH || frame_h != PLAYER_HEIGHT) {
            SDL_Log("Note: walk frame size differs from PLAYER_WIDTH/HEIGHT (%d x %d)\n", PLAYER_WIDTH, PLAYER_HEIGHT);
        }
    }

    if (jump_player.tex_jumpplayer) {
        int w=0,h=0; SDL_QueryTexture(jump_player.tex_jumpplayer, NULL, NULL, &w, &h);
        int frame_w = idle_frame_w ? idle_frame_w : ((PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : w);
        if (frame_w <= 0) frame_w = w;
        int detected_frames = (frame_w > 0) ? (w / frame_w) : 1;
        if (detected_frames <= 0) detected_frames = 1;
        int frame_h = idle_frame_h ? idle_frame_h : h;
        Animation_Init(&jump_player.jump_anim, frame_w, frame_h, detected_frames, FRAME_DURATION_PLAYER);
        SDL_Log("ChangePlayerSkin: jump texture %dx%d -> frames=%d frame_size=%dx%d\n", w, h, detected_frames, frame_w, frame_h);
        if (frame_w != PLAYER_WIDTH || frame_h != PLAYER_HEIGHT) {
            SDL_Log("Note: jump frame size differs from PLAYER_WIDTH/HEIGHT (%d x %d)\n", PLAYER_WIDTH, PLAYER_HEIGHT);
        }
    }

    /*
    if (shoot_player.tex_shootplayer){
        int w=0,h=0; SDL_QueryTexture(shoot_player.tex_shootplayer, NULL, NULL, &w, &h);
        int frame_w = idle_frame_w ? idle_frame_w : ((PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : w);
        if (frame_w <= 0) frame_w = w;
        int detected_frames = (frame_w > 0) ? (w / frame_w) : 1;
        if (detected_frames <= 0) detected_frames = 1;
        int frame_h = idle_frame_h ? idle_frame_h : h;
        Animation_Init(&shoot_player.shoot_anim, frame_w, frame_h, detected_frames, FRAME_DURATION_PLAYER);
        SDL_Log("ChangePlayerSkin: shoot texture %dx%d -> frames=%d frame_size=%dx%d\n", w, h, detected_frames, frame_w, frame_h);
        if (frame_w != PLAYER_WIDTH || frame_h != PLAYER_HEIGHT) {
            SDL_Log("Note: shoot frame size differs from PLAYER_WIDTH/HEIGHT (%d x %d)\n", PLAYER_WIDTH, PLAYER_HEIGHT);
        }
    }

    if(shoot_walk_player.tex_shootwalkplayer){
        int w=0,h=0; SDL_QueryTexture(shoot_walk_player.tex_shootwalkplayer, NULL, NULL, &w, &h);
        int frame_w = idle_frame_w ? idle_frame_w : ((PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : w);
        if (frame_w <= 0) frame_w = w;
        int detected_frames = (frame_w > 0) ? (w / frame_w) : 1;
        if (detected_frames <= 0) detected_frames = 1;
        int frame_h = idle_frame_h ? idle_frame_h : h;
        Animation_Init(&shoot_walk_player.shootwalk_anim, frame_w, frame_h, detected_frames, FRAME_DURATION_PLAYER);
        SDL_Log("ChangePlayerSkin: shoot walk texture %dx%d -> frames=%d frame_size=%dx%d\n", w, h, detected_frames, frame_w, frame_h);
        if (frame_w != PLAYER_WIDTH || frame_h != PLAYER_HEIGHT) {
            SDL_Log("Note: shoot walk frame size differs from PLAYER_WIDTH/HEIGHT (%d x %d)\n", PLAYER_WIDTH, PLAYER_HEIGHT);
        }
    }
    */
   
    /* Auto-adjust base speed based on skin name keywords (simple heuristic).
       If you want explicit control, call SetPlayerBaseSpeed() from UI after ChangePlayerSkin. */
    float suggested_speed = default_base_player_speed; /* default: base relative to original default */
    const char* paths[] = { idle_path, walk_path, jump_path };
    bool found_metal = false;
    for (int i = 0; i < 3; ++i) {
        const char* p = paths[i];
        if (!p) continue;
        if (strstr(p, "METAL-SONIC") || strstr(p, "METALSONIC")) {
            suggested_speed = default_base_player_speed * 2.2f; /* much faster */
            found_metal = true;
            break;
        }
        if (strstr(p, "SONIC")) {
            suggested_speed = default_base_player_speed * 2.0f; /* very fast */
            break;
        }
        if (strstr(p, "FAST")) {
            suggested_speed = default_base_player_speed * 1.7f; /* faster */
            break;
        }
        if (strstr(p, "SLOW") || strstr(p, "HEAVY")) {
            suggested_speed = default_base_player_speed * 0.8f; /* slower */
            break;
        }else{
        }
    }
    if (suggested_speed != base_player_speed) {
        /* compute skin multiplier relative to default base and store it */
        skin_speed_multiplier = suggested_speed / default_base_player_speed;
        base_player_speed = default_base_player_speed * skin_speed_multiplier;
        player_speed = base_player_speed;
        SDL_Log("ChangePlayerSkin: adjusted base speed to %.2f (skin_multiplier=%.2f) based on skin name\n", base_player_speed, skin_speed_multiplier);
    }

    /* remember whether the current skin is metal for shift behavior */
    skin_is_metal = found_metal;
    /* expose skin_is_metal by storing in skin_speed_multiplier > threshold is also possible */
    /* If metal skin detected, try to load the provided shoot spritesheet */
    if (found_metal) {
        ChangePlayerShootSkin("sprites/atake-METAL-SONIC.png");
        ChangePlayerShootWalkSkin("sprites/atake2-METAL-SONIC.png");
        SDL_Log("ChangePlayerSkin: auto-loaded shoot spritesheets for metal skin\n");
    }else{
        ChangePlayerShootSkin("sprites/BANG_dessert-spritesheet.png");
        ChangePlayerShootWalkSkin("sprites/BANG_dessert-spritesheet.png");
    }
    
}

void CleanTextureShoot()
{

    //Clean shoot_player
    if(shoot_player.tex_shootplayer)
    {
        SDL_DestroyTexture(shoot_player.tex_shootplayer);
        shoot_player.tex_shootplayer = NULL;
    }

    if(shoot_player.tmp_surf_shootplayer)
    {
        SDL_FreeSurface(shoot_player.tmp_surf_shootplayer);
        shoot_player.tmp_surf_shootplayer = NULL;
    }

    //Clean shoot_walk_player
    if(shoot_walk_player.tex_shootwalkplayer)
    {
        SDL_DestroyTexture(shoot_walk_player.tex_shootwalkplayer);
        shoot_walk_player.tex_shootwalkplayer = NULL;
    }

    if(shoot_walk_player.tmp_surf_shootwalkplayer)
    {
        SDL_FreeSurface(shoot_walk_player.tmp_surf_shootwalkplayer);
        shoot_walk_player.tmp_surf_shootwalkplayer = NULL;
    }

    //Reset anims
    Animation_Reset(&shoot_player.shoot_anim);
    Animation_Reset(&shoot_walk_player.shootwalk_anim);
}

/* Load/replace the shoot-walking spritesheet to be used when player shoots while walking */
void ChangePlayerShootWalkSkin(const char* shoot_walk_path)
{
    if (!shoot_walk_path || !*shoot_walk_path) return;
    if (shoot_walk_player.tex_shootwalkplayer) { SDL_DestroyTexture(shoot_walk_player.tex_shootwalkplayer); shoot_walk_player.tex_shootwalkplayer = NULL; }
    if (shoot_walk_player.tmp_surf_shootwalkplayer) { SDL_FreeSurface(shoot_walk_player.tmp_surf_shootwalkplayer); shoot_walk_player.tmp_surf_shootwalkplayer = NULL; }
    shoot_walk_player.tmp_surf_shootwalkplayer = IMG_Load(shoot_walk_path);
    if (shoot_walk_player.tmp_surf_shootwalkplayer) {
        shoot_walk_player.tex_shootwalkplayer = SDL_CreateTextureFromSurface(renderer, shoot_walk_player.tmp_surf_shootwalkplayer);
        SDL_FreeSurface(shoot_walk_player.tmp_surf_shootwalkplayer);
        shoot_walk_player.tmp_surf_shootwalkplayer = NULL;
        int w=0,h=0; SDL_QueryTexture(shoot_walk_player.tex_shootwalkplayer, NULL, NULL, &w, &h);
        int frames = (PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : 1;
        if (frames <= 0) frames = 1;
        if(skin_is_metal){
            Animation_Init(&shoot_walk_player.shootwalk_anim, PLAYER_WIDTH, PLAYER_HEIGHT, frames, FRAME_DURATION_PLAYER/2);
        }else{
            Animation_Init(&shoot_walk_player.shootwalk_anim, PLAYER_WIDTH, PLAYER_HEIGHT, frames, FRAME_DURATION_PLAYER);
        }
            
        SDL_Log("ChangePlayerShootWalkSkin: loaded shoot-walk skin '%s' frames=%d\n", shoot_walk_path, frames);
    } else {
        SDL_Log("ChangePlayerShootWalkSkin: failed to load '%s'\n", shoot_walk_path);
    }
}

/* Load/replace the shoot spritesheet to be used when player shoots */
void ChangePlayerShootSkin(const char* shoot_path)
{
    if (!shoot_path || !*shoot_path) return;
    if (shoot_player.tex_shootplayer) { SDL_DestroyTexture(shoot_player.tex_shootplayer); shoot_player.tex_shootplayer = NULL; }
    if (shoot_player.tmp_surf_shootplayer) { SDL_FreeSurface(shoot_player.tmp_surf_shootplayer); shoot_player.tmp_surf_shootplayer = NULL; }
    shoot_player.tmp_surf_shootplayer = IMG_Load(shoot_path);
    if (shoot_player.tmp_surf_shootplayer) {
        shoot_player.tex_shootplayer = SDL_CreateTextureFromSurface(renderer, shoot_player.tmp_surf_shootplayer);
        SDL_FreeSurface(shoot_player.tmp_surf_shootplayer);
        shoot_player.tmp_surf_shootplayer = NULL;
        int w=0,h=0; SDL_QueryTexture(shoot_player.tex_shootplayer, NULL, NULL, &w, &h);
        int frames = (PLAYER_WIDTH > 0) ? (w / PLAYER_WIDTH) : 1;
        if (frames <= 0) frames = 1;
        if(skin_is_metal)
            Animation_Init(&shoot_player.shoot_anim, PLAYER_WIDTH, PLAYER_HEIGHT, frames, FRAME_DURATION_PLAYER/2);
        else
            Animation_Init(&shoot_player.shoot_anim, PLAYER_WIDTH, PLAYER_HEIGHT, frames, FRAME_DURATION_PLAYER);
        SDL_Log("ChangePlayerShootSkin: loaded shoot skin '%s' frames=%d\n", shoot_path, frames);
    } else {
        SDL_Log("ChangePlayerShootSkin: failed to load '%s'\n", shoot_path);
    }
}

/* shoot spritesheet support removed */

void SetPlayerBaseSpeed(float new_base_speed)
{
    if (new_base_speed <= 0.0f) return;
    base_player_speed = new_base_speed;
    /* update skin multiplier to match new base relative to default */
    skin_speed_multiplier = base_player_speed / default_base_player_speed;
    player_speed = base_player_speed; /* reset current speed to new base */
    SDL_Log("SetPlayerBaseSpeed: base_player_speed=%.2f player_speed=%.2f skin_multiplier=%.2f\n", base_player_speed, player_speed, skin_speed_multiplier);
}

/* Start the shoot animation: switch player state to SHOOT and initialize animation */
/* single-play keep for compatibility: start single animation (non-looping) */
void AnimatePlayerShoot()
{
    if (!shoot_player.tex_shootplayer) return;
    shoot_prev_state = states_player;
    states_player = SHOOT;
    Animation* anim = &shoot_player.shoot_anim;
    anim->current_frame = 0;
    anim->last_update_time = SDL_GetTicks();
    anim->src_rect.x = 0;
    anim->src_rect.y = 0;
    anim->src_rect.w = anim->frame_width;
    anim->src_rect.h = anim->frame_height;
    shoot_anim_playing = true;
    shoot_looping = false;
}

void UpdatePlayerShootAnim(void)
{
    if (!shoot_anim_playing) return;
    Animation* anim = (shoot_anim_type == 1) ? &shoot_walk_player.shootwalk_anim : &shoot_player.shoot_anim;
    Uint32 now = SDL_GetTicks();
    if (now > anim->last_update_time + anim->frame_duration) {
        anim->current_frame++;
        if (anim->current_frame >= anim->total_frames) {
            if (shoot_looping) {
                /* loop */
                anim->current_frame = 0;
                anim->src_rect.x = 0;
                anim->last_update_time = now;
            } else {
                /* finished single-play */
                shoot_anim_playing = false;
                states_player = shoot_prev_state;
                anim->current_frame = 0;
                anim->src_rect.x = 0;
            }
        } else {
            anim->src_rect.x = anim->current_frame * anim->frame_width;
            anim->last_update_time = now;
        }
    }
}

float GetShiftMultiplierForSkin(void)
{
    /* For metal skin allow stronger sprint; otherwise reduce sprint multiplier */
    if (skin_is_metal) return 1.8f;
    return 2.0f; /* reduced sprint multiplier for base/non-metal skins */
}

float GetProjectileDamageMultiplier(void)
{
    /* Metal sonic skin deals increased projectile damage */
    if (skin_is_metal) return 1.5f; /* 50% more damage */
    return 1.0f;
}

/* shoot animation functions removed */

void RenderIdlePlayerAnim(SDL_RendererFlip flip_type) {
    Animation_Update(&idle_player.idle_anim);
    SDL_Rect* src_rect = Animation_GetSourceRect(&idle_player.idle_anim);
    
    idle_player.dest_idleplayer.x = (int)(position_x + (PLAYER_WIDTH - src_rect->w) / 2);
    /* anchor to player's feet: keep bottom aligned with `position_y + PLAYER_HEIGHT` */
    idle_player.dest_idleplayer.w = src_rect->w;
    idle_player.dest_idleplayer.h = src_rect->h;
    idle_player.dest_idleplayer.y = position_y + (PLAYER_HEIGHT - src_rect->h);

    SDL_Rect drawDest = idle_player.dest_idleplayer;
    drawDest.x -= backgroundSrcRect.x;
    drawDest.y -= backgroundSrcRect.y;
    SDL_RenderCopyEx(renderer, idle_player.tex_idleplayer, src_rect, 
                    &drawDest, 0.0, NULL, flip_type);
}

void PlayerForward()
{
    position_x += player_speed * deltaTime;
    int rightLimit = backgroundImgW - PLAYER_WIDTH;
    if (worldBarrierX > 0) {
        int barrierLimit = worldBarrierX - PLAYER_WIDTH;
        if (barrierLimit < rightLimit) rightLimit = barrierLimit;
    }
    if(position_x > rightLimit){
        position_x = rightLimit;
    }
    UpdatePlayerHitbox();
}

void PlayerWalkAnim(SDL_RendererFlip flip_type)
{
    Animation_Update(&walk_player.walk_anim);
    SDL_Rect* src_rect = Animation_GetSourceRect(&walk_player.walk_anim);

    walk_player.dest_walkplayer.x = (int)(position_x + (PLAYER_WIDTH - src_rect->w) / 2);
    /* align bottom of frame with player's base (position_y + PLAYER_HEIGHT) */
    walk_player.dest_walkplayer.y = position_y + (PLAYER_HEIGHT - src_rect->h);
    walk_player.dest_walkplayer.w = src_rect->w;
    walk_player.dest_walkplayer.h = src_rect->h;

    SDL_Rect drawDest = walk_player.dest_walkplayer;
    drawDest.x -= backgroundSrcRect.x;
    drawDest.y -= backgroundSrcRect.y;
    SDL_RenderCopyEx(renderer, walk_player.tex_walkplayer, src_rect,
                     &drawDest, 0.0, NULL, flip_type);
}

void PlayerJumpAnim(SDL_RendererFlip flip_type)
{
    Animation_Update(&jump_player.jump_anim);
    SDL_Rect* src_rect = Animation_GetSourceRect(&jump_player.jump_anim);

    /* Add a bit on the left and crop a bit on the right to adjust framing (inverted):3 */
    SDL_Rect src_mod = *src_rect;
    int tex_w = 0, tex_h = 0;
    if (jump_player.tex_jumpplayer) SDL_QueryTexture(jump_player.tex_jumpplayer, NULL, NULL, &tex_w, &tex_h);

    const int left_extra = 6;  /* pixels to include to the left of frame */
    const int right_crop = 6;  /* pixels to remove from right of frame */

    /* try to move source x leftwards by left_extra (if possible) */
    int new_x = src_mod.x - left_extra;
    int applied_left = left_extra;
    if (new_x < 0) { applied_left = src_mod.x; new_x = 0; }
    src_mod.x = new_x;

    src_mod.w = src_rect->w + applied_left - right_crop;
    if (src_mod.w < 1) src_mod.w = 1;
    if (tex_w > 0 && src_mod.x + src_mod.w > tex_w) src_mod.w = tex_w - src_mod.x;

    jump_player.dest_jumpplayer.x = (int)(position_x + (PLAYER_WIDTH - src_mod.w) / 2);
    /* align bottom of frame with player's base (position_y + PLAYER_HEIGHT) */
    jump_player.dest_jumpplayer.y = position_y + (PLAYER_HEIGHT - src_mod.h);
    jump_player.dest_jumpplayer.w = src_mod.w;
    jump_player.dest_jumpplayer.h = src_mod.h;

    SDL_Rect drawDest = jump_player.dest_jumpplayer;
    drawDest.x -= backgroundSrcRect.x;
    drawDest.y -= backgroundSrcRect.y;
    SDL_RenderCopyEx(renderer, jump_player.tex_jumpplayer, &src_mod,
                     &drawDest, 0.0, NULL, flip_type);
}

void RenderPlayer(SDL_RendererFlip flip_type)
{
    if (states_player == SHOOT) {
        /* choose appropriate shoot texture/anim based on type */
        Animation* anim = (shoot_anim_type == 1) ? &shoot_walk_player.shootwalk_anim : &shoot_player.shoot_anim;
        SDL_Texture* tex = (shoot_anim_type == 1) ? shoot_walk_player.tex_shootwalkplayer : shoot_player.tex_shootplayer;
        if (tex) {
            SDL_Rect* src_rect = Animation_GetSourceRect(anim);
            /* target display size = slightly smaller than idle frame size */
            SDL_Rect* idle_src = Animation_GetSourceRect(&idle_player.idle_anim);
            const float SHOOT_SCALE = 1.00f; /* normal shoot scale */
            const float SHOOT_WALK_SCALE = 0.98f; /* larger for walk-shoot */
            float scale = (shoot_anim_type == 1) ? SHOOT_WALK_SCALE : SHOOT_SCALE;
            int target_w = idle_src ? (int)(idle_src->w * scale) : (int)(PLAYER_WIDTH * scale);
            int target_h = idle_src ? (int)(idle_src->h * scale) : (int)(PLAYER_HEIGHT * scale);
            if (target_w < 1) target_w = 1;
            if (target_h < 1) target_h = 1;

            /* Set destination rect to scaled target size */
            if (shoot_anim_type == 1) {
                shoot_walk_player.dest_shootwalkplayer.w = target_w;
                shoot_walk_player.dest_shootwalkplayer.h = target_h;
                shoot_walk_player.dest_shootwalkplayer.x = (int)(position_x + (PLAYER_WIDTH - target_w) / 2);
                shoot_walk_player.dest_shootwalkplayer.y = position_y + (PLAYER_HEIGHT - target_h);
                SDL_Rect drawDest = shoot_walk_player.dest_shootwalkplayer;
                drawDest.x -= backgroundSrcRect.x;
                drawDest.y -= backgroundSrcRect.y;
                SDL_RenderCopyEx(renderer, tex, src_rect,
                                 &drawDest, 0.0, NULL, flip_type);
            } else {
                shoot_player.dest_shootplayer.w = target_w;
                shoot_player.dest_shootplayer.h = target_h;
                shoot_player.dest_shootplayer.x = (int)(position_x + (PLAYER_WIDTH - target_w) / 2);
                shoot_player.dest_shootplayer.y = position_y + (PLAYER_HEIGHT - target_h);
                SDL_Rect drawDest = shoot_player.dest_shootplayer;
                drawDest.x -= backgroundSrcRect.x;
                drawDest.y -= backgroundSrcRect.y;
                SDL_RenderCopyEx(renderer, tex, src_rect,
                                 &drawDest, 0.0, NULL, flip_type);
            }
            return;
        }
    }

    if(states_player == IDLE){
        RenderIdlePlayerAnim(flip_type);
    }else if(states_player == WALK){
        PlayerWalkAnim(flip_type);
    } else if(states_player == JUMP){
        PlayerJumpAnim(flip_type);
    }
}

void PlayerJump()
{
    velocity_y += gravity * deltaTime; 

    position_y += velocity_y * deltaTime;

    if(position_y >= POS_Y)
    {
        position_y = POS_Y;
        is_jumping = false;
        velocity_y = 0.0f;
    }
    UpdatePlayerHitbox();
}

void PlayerBackward()
{
    position_x -= player_speed * deltaTime;
    int leftLimit = 0;
    if (worldBarrierLeftX >= 0) {
        leftLimit = worldBarrierLeftX + WORLD_BARRIER_WIDTH;
    }
    if(position_x < leftLimit){
        position_x = leftLimit;
    }
    UpdatePlayerHitbox();
}

/*
 * Set a multiplier for player speed. multiplier = 1.0 -> normal speed.
 * Example: SetPlayerSpeedMultiplier(1.8f) to increase by 80%.
 */
void SetPlayerSpeedMultiplier(float multiplier)
{
    if (multiplier <= 0.0f) {
        /* avoid invalid speeds */
        player_speed = base_player_speed;
    } else {
        /* apply multiplier on top of original base and skin multiplier explicitly */
        player_speed = default_base_player_speed * skin_speed_multiplier * multiplier;
    }
}

/* restaura la velocidad del jugador a la normal*/
void ResetPlayerSpeed(void)
{
    player_speed = default_base_player_speed * skin_speed_multiplier;
}

void CheckIfPlayerIsDead()
{
    if(health <= 0)
    {
        SDL_Log("Player is dead!\n");
        exit(0);
    }
}

float GetPositionPlayerX(void)
{
    return position_x;
}

float GetPositionPlayerY(void)
{
    return position_y;
}
