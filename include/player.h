#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    /*IDLE PLAYER*/
    SDL_Surface* tmp_surf_idleplayer;
    SDL_Texture* tex_idleplayer;
    SDL_Rect src_idleplayer;
    SDL_Rect dest_idleplayer;
} IDLE_PLAYER;

typedef struct{
    /*WALK PLAYER*/
    SDL_Surface* tmp_surf_walkplayer;
    SDL_Texture* tex_walkplayer;
    SDL_Rect src_walkplayer;
    SDL_Rect dest_walkplayer;
} WALK_PLAYER;

typedef struct{
    /*JUMP PLAYER*/
    SDL_Surface* tmp_surf_jumpplayer;
    SDL_Texture* tex_jumpplayer;
    SDL_Rect src_jumpplayer;
    SDL_Rect dest_jumpplayer;
} JUMP_PLAYER;

typedef enum{
    IDLE,
    WALK,
    JUMP
} StatesPlayer;

#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 64

#define IDLE_FRAMES 2
#define WALK_FRAMES 11
#define JUMP_FRAMES 2

/*LOAD SPRITES*/
void LoadSpritesPlayer();
//void AnimatePlayerShoot();

/*Movement*/
void PlayerForward();
void PlayerBackward();
void PlayerJump();

/*RENDER AND ANIMATION*/
void RenderPlayer(SDL_RendererFlip flip_type);
void PlayerWalkAnim(SDL_RendererFlip flip_type);
void UpdateANIM(int frames);
void Update_IDLE();
void RenderIdlePlayerAnim(SDL_RendererFlip flip_type);
void SetPlayerSpeedMultiplier(float multiplier);
void ResetPlayerSpeed(void);

#endif // PLAYER_H