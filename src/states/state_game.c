/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: state_menu.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/*
STATE GAME :D
*/

#include "states.h"
#include "global_vars.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"

SDL_Texture* backgroundTexture = NULL; //Background texture
SDL_Rect backgroundRect;
extern StatesPlayer states_player;
SDL_RendererFlip player_flip = SDL_FLIP_NONE;

int Init_State_Game()
{
    /*Load BACKGROUND test main menu test*/
    SDL_Surface* background_surface = IMG_Load("sprites/CHIP8-PS2-BACKGROUND.png");

    if(background_surface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/CHIP8-PS2-BACKGROUND.png", IMG_GetError());

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface); //Convert surface to texture
    SDL_FreeSurface(background_surface); //Free loaded surface

    //Background texture properties
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = 640;
    backgroundRect.h = 480;

    LoadSpritesPlayer(); //Load player sprites

    return 0;
}

int Update_State_Game()
{
    while(1) //Main loop(TESTING)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_Quit();
                return 0;
            }
        }

        states_player = IDLE;

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(state[SDL_SCANCODE_ESCAPE]){
            SDL_Quit();
            return 0;
        }

        if(state[SDL_SCANCODE_LEFT]){
            states_player = WALK;
            player_flip = SDL_FLIP_HORIZONTAL;
            PlayerBackward();
        }

        if(state[SDL_SCANCODE_RIGHT]){
            states_player = WALK;
            player_flip = SDL_FLIP_NONE;
            PlayerForward();
        }

        /*TESTING*/
        //SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect); /*TEXTURE TEST :)*/

        //AnimatePlayerShoot(); //Animate player shooting(TESTING)
        RenderPlayer(player_flip); //Render player

        SDL_RenderPresent(renderer);
    }
    return 0;
}