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

void LoadSpritesPlayer();
//void AnimatePlayerShoot();
void Update_IDLE();
void RenderIdlePlayerAnim();
void PlayerForward();
void PlayerBackward();

#endif // PLAYER_H