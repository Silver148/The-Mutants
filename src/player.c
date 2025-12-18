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

#define POS_Y 350
#define POS_X 100

/*IDLE PLAYER*/
IDLE_PLAYER idle_player;

/*WALK PLAYER*/
WALK_PLAYER walk_player;

/*JUMP PLAYER*/
JUMP_PLAYER jump_player;

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

    /* Auto-adjust base speed based on skin name keywords (simple heuristic).
       If you want explicit control, call SetPlayerBaseSpeed() from UI after ChangePlayerSkin. */
    float suggested_speed = base_player_speed; /* default keep current base :b*/
    const char* paths[] = { idle_path, walk_path, jump_path };
    bool found_metal = false;
    for (int i = 0; i < 3; ++i) {
        const char* p = paths[i];
        if (!p) continue;
        if (strstr(p, "METAL-SONIC") || strstr(p, "METALSONIC")) {
            suggested_speed = base_player_speed * 2.2f; /* much faster */
            found_metal = true;
            break;
        }
        if (strstr(p, "SONIC")) {
            suggested_speed = base_player_speed * 2.0f; /* very fast */
            break;
        }
        if (strstr(p, "FAST")) {
            suggested_speed = base_player_speed * 1.7f; /* faster */
            break;
        }
        if (strstr(p, "SLOW") || strstr(p, "HEAVY")) {
            suggested_speed = base_player_speed * 0.8f; /* slower */
            break;
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

void SetPlayerBaseSpeed(float new_base_speed)
{
    if (new_base_speed <= 0.0f) return;
    base_player_speed = new_base_speed;
    /* update skin multiplier to match new base relative to default */
    skin_speed_multiplier = base_player_speed / default_base_player_speed;
    player_speed = base_player_speed; /* reset current speed to new base */
    SDL_Log("SetPlayerBaseSpeed: base_player_speed=%.2f player_speed=%.2f skin_multiplier=%.2f\n", base_player_speed, player_speed, skin_speed_multiplier);
}

float GetShiftMultiplierForSkin(void)
{
    /* For metal skin allow stronger sprint; otherwise reduce sprint multiplier */
    if (skin_is_metal) return 1.8f;
    return 1.4f; /* reduced sprint multiplier for base/non-metal skins */
}

void AnimatePlayerShoot()
{
    //NOTHING YET :D
}

void RenderIdlePlayerAnim(SDL_RendererFlip flip_type) {
    Animation_Update(&idle_player.idle_anim);
    SDL_Rect* src_rect = Animation_GetSourceRect(&idle_player.idle_anim);
    
    idle_player.dest_idleplayer.x = (int)(position_x + (PLAYER_WIDTH - src_rect->w) / 2);
    /* anchor to player's feet: keep bottom aligned with `position_y + PLAYER_HEIGHT` */
    idle_player.dest_idleplayer.w = src_rect->w;
    idle_player.dest_idleplayer.h = src_rect->h;
    idle_player.dest_idleplayer.y = position_y + (PLAYER_HEIGHT - src_rect->h);

    SDL_RenderCopyEx(renderer, idle_player.tex_idleplayer, src_rect, 
                    &idle_player.dest_idleplayer, 0.0, NULL, flip_type);
}

void PlayerForward()
{
    position_x += player_speed * deltaTime;

    if(position_x > 640 - PLAYER_WIDTH){
        position_x = 640 - PLAYER_WIDTH;
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

    SDL_RenderCopyEx(renderer, walk_player.tex_walkplayer, src_rect,
                     &walk_player.dest_walkplayer, 0.0, NULL, flip_type);
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

    SDL_RenderCopyEx(renderer, jump_player.tex_jumpplayer, &src_mod,
                     &jump_player.dest_jumpplayer, 0.0, NULL, flip_type);
}

void RenderPlayer(SDL_RendererFlip flip_type)
{
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

    if(position_x < 0){
        position_x = 0;
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
