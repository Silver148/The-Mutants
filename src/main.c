/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: main.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

//COMO TAN MUCHACHO >:D

Copyright 2025
*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __MINGW32__
#define main SDL_main 
#endif

//Func Main
int main(int argc, char* argv[])
{   
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    /*Initialize SDL*/
    if(SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    /*Initialize SDL_image*/
    if(IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    /*Create window*/
    window = SDL_CreateWindow("The Mutant's(TEST)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN); //Window 640x480
    if(window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    /*Create renderer for window*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

    /*Load BACKGROUND test main menu test*/
    SDL_Surface* background_surface = IMG_Load("sprites/CHIP8-PS2-BACKGROUND.png");

    /*Load player sprite*/
    SDL_Surface* player_sprite = IMG_Load("sprites/player_shoot_frame-0.png");

    if(background_surface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/CHIP8-PS2-BACKGROUND.png", IMG_GetError());

    if(player_sprite == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/player_shoot_frame-0.png", IMG_GetError());

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface); //Convert surface to texture
    SDL_FreeSurface(background_surface); //Free loaded surface

    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, player_sprite); //Convert surface to texture
    SDL_FreeSurface(player_sprite); //Free loaded surface

    //Background texture properties
    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = 640;
    backgroundRect.h = 480;

    //Player texture properties
    SDL_Rect playerRect;
    playerRect.x = 288; //Center of the screen
    playerRect.y = 208; //Center of the screen
    playerRect.w = 64; //Width of the sprite
    playerRect.h = 64; //Height of the sprite

    while(1) //Main loop(TESTING)
    {
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_KEYDOWN: //Key pressed
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: /*EXIT*/
                        SDL_Quit();
                        return 0;

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
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect); /*PLAYER TEST :)*/

        SDL_RenderPresent(renderer);
    }

    return 0;
}

// End func main
