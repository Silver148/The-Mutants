/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: player.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

//COMO TAN MUCHACHO >:D

Copyright 2025
*/

//The_Light y yo(silverhacker) haremos el movimiento del player

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PLAYER_SHOOT_FRAMES_COUNT 4
#define FRAME_DELAY 250

int player_shoot_frames_counts = PLAYER_SHOOT_FRAMES_COUNT; //Number of frames in the shooting animation
SDL_Surface* player_shoot_frames[PLAYER_SHOOT_FRAMES_COUNT]; //Array to hold the shooting frames
SDL_Texture* player_shoot_frames_texture[PLAYER_SHOOT_FRAMES_COUNT];
SDL_Rect playerRect = { 288, 208, 64, 64 }; //Player propierties
extern SDL_Renderer* renderer;
extern SDL_Window* window;

void LoadSpritesPlayer()
{   
    /*LOAD SHOOT FRAMES*/
    player_shoot_frames[0] = IMG_Load("sprites/player_shoot_frame-0.png");
    player_shoot_frames[1] = IMG_Load("sprites/player_shoot_frame-1.png");
    player_shoot_frames[2] = IMG_Load("sprites/player_shoot_frame-2.png");
    player_shoot_frames[3] = IMG_Load("sprites/player_shoot_frame-3.png");

    for(int i = 0; i < player_shoot_frames_counts; i++)
    {
        if(player_shoot_frames[i] == NULL)
        {
            printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/player_shoot_frame-0.png", IMG_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            exit(1);
        }
    }

    for(int i = 0; i < player_shoot_frames_counts; i++)
    {
        player_shoot_frames_texture[i] = SDL_CreateTextureFromSurface(renderer, player_shoot_frames[i]);
        SDL_FreeSurface(player_shoot_frames[i]);
    }
}

void AnimatePlayerShoot()
{
    static int frame = 0; // actual frame
    static Uint32 lastFrameTime = 0; // last frame time

    // Get current time
    Uint32 currentTime = SDL_GetTicks();

    //Change frame every FRAME_DELAY milliseconds
    if (currentTime > lastFrameTime + FRAME_DELAY) {
        frame = (frame + 1) % PLAYER_SHOOT_FRAMES_COUNT; //Change frame
        lastFrameTime = currentTime; //Update last frame time
    }

    //Draw animation shoot player :)
    SDL_RenderCopy(renderer, player_shoot_frames_texture[frame], NULL, &playerRect);
}
