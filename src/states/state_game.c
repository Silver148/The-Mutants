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
        if(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_KEYDOWN: //Key pressed
                    switch(e.key.keysym.sym) /*TEST CONTROLLER :D*/
                    {
                        case SDLK_ESCAPE: /*EXIT*/
                            SDL_Quit();
                            return 0; //Exit main loop
                        break;

                        case SDLK_RIGHT: /*MOVE PLAYER RIGHT*/
                            //PlayerForward();
                        break;

                        case SDLK_LEFT: /*MOVE PLAYER LEFT*/
                            //PlayerBackward();
                        break;
                    }
                    break;

                case SDL_QUIT: /*EXIT*/
                    SDL_Quit();
                    return 0;
                    break;
            }
        }

        /*TESTING*/
        //SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect); /*TEXTURE TEST :)*/

        //AnimatePlayerShoot(); //Animate player shooting(TESTING)
        RenderIdlePlayerAnim(); //Render idle player animation

        SDL_RenderPresent(renderer);
    }
    return 0;
}