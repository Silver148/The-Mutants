#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "anim_manager.h"
#define FRAME_DURATION_PLAYER 150

typedef struct{
    /*IDLE PLAYER*/
    SDL_Surface* tmp_surf_idleplayer;
    SDL_Texture* tex_idleplayer;
    SDL_Rect src_idleplayer;
    SDL_Rect dest_idleplayer;
    Animation idle_anim;
} IDLE_PLAYER;

typedef struct{
    /*WALK PLAYER*/
    SDL_Surface* tmp_surf_walkplayer;
    SDL_Texture* tex_walkplayer;
    SDL_Rect src_walkplayer;
    SDL_Rect dest_walkplayer;
    Animation walk_anim;
} WALK_PLAYER;

typedef struct{
    /*JUMP PLAYER*/
    SDL_Surface* tmp_surf_jumpplayer;
    SDL_Texture* tex_jumpplayer;
    SDL_Rect src_jumpplayer;
    SDL_Rect dest_jumpplayer;
    Animation jump_anim;
} JUMP_PLAYER;

typedef struct{
    /*SHOOT PLAYER*/
    SDL_Surface* tmp_surf_shootplayer;
    SDL_Texture* tex_shootplayer;
    SDL_Rect src_shootplayer;
    SDL_Rect dest_shootplayer;
    Animation shoot_anim;
} SHOOT_PLAYER;

typedef struct{
    /*SHOOT WALK PLAYER*/
    SDL_Surface* tmp_surf_shootwalkplayer;
    SDL_Texture* tex_shootwalkplayer;
    SDL_Rect src_shootwalkplayer;
    SDL_Rect dest_shootwalkplayer;
    Animation shootwalk_anim;
} SHOOT_WALK_PLAYER;

/* Global player instances (defined in player.c) */
extern IDLE_PLAYER idle_player;
extern WALK_PLAYER walk_player;
extern JUMP_PLAYER jump_player;
extern SHOOT_PLAYER shoot_player;
extern SHOOT_WALK_PLAYER shoot_walk_player;
extern int PLAYER_WIDTH;
extern int PLAYER_HEIGHT;

typedef enum{
    IDLE,
    WALK,
    JUMP,
    SHOOT
} StatesPlayer;

#define IDLE_FRAMES 2
#define WALK_FRAMES 11
#define JUMP_FRAMES 2

/*LOAD SPRITES*/
void LoadSpritesPlayer();
/* Change player skin at runtime: provide paths for idle, walk and jump sprites */
void ChangePlayerSkin(const char* idle_path, const char* walk_path, const char* jump_path);
/* Set a custom spritesheet for the player's shooting animation (optional) */
void ChangePlayerShootSkin(const char* shoot_path);
void ChangePlayerShootWalkSkin(const char* shoot_walk_path);
/* Adjust base player speed (pixels/sec). Use this to change speed when skin changes. */
void SetPlayerBaseSpeed(float new_base_speed);
/* Returns the shift (sprint) multiplier appropriate for the current skin. */
float GetShiftMultiplierForSkin(void);
/* Returns projectile damage multiplier for the current skin (1.0 = normal). */
float GetProjectileDamageMultiplier(void);
/* shoot animation has been removed */

/*Movement*/
void PlayerForward();
void PlayerBackward();
void PlayerJump();

/*RENDER AND ANIMATION*/
void RenderPlayer(SDL_RendererFlip flip_type);
void PlayerWalkAnim(SDL_RendererFlip flip_type);
/* Trigger shoot animation (plays once) */
void AnimatePlayerShoot(void);
/* update shoot animation each frame */
void UpdatePlayerShootAnim(void);
/* Returns true while the player's shoot animation is playing */
bool IsPlayerShooting(void);
/* Start/stop continuous shooting animation while key held */
void StartPlayerShooting(void);
void StartPlayerShootingWalk(void);
void StopPlayerShooting(void);
void Update_IDLE();
void RenderIdlePlayerAnim(SDL_RendererFlip flip_type);
void SetPlayerSpeedMultiplier(float multiplier);
void ResetPlayerSpeed(void);
void CheckIfPlayerIsDead();

/*POSITION X AND Y*/
float GetPositionPlayerX(void);
float GetPositionPlayerY(void);

typedef struct {
    float x, y;
    int w, h;
} Hitbox;

Hitbox GetPlayerHitbox(void);
void UpdatePlayerHitbox(void);

void CleanupPlayer();

#endif // PLAYER_H