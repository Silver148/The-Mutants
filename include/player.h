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

typedef enum{
    IDLE,
    WALK
} StatesPlayer;

void LoadSpritesPlayer();
//void AnimatePlayerShoot();
void Update_IDLE();
void RenderIdlePlayerAnim(SDL_RendererFlip flip_type);
void PlayerForward();
void PlayerBackward();
void RenderPlayer(SDL_RendererFlip flip_type);

#endif // PLAYER_H