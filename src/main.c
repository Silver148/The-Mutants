/*
THE MUTANTS

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Montana-call)
File: main.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

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
    window = SDL_CreateWindow("The Mutant's", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN); //Window 640x480
    if(window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    /*Create renderer for window*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

    /*Load PNG texture main menu test*/
    SDL_Surface* loadedSurface = IMG_Load("sprites/CHIP8-PS2-BACKGROUND.png");

    if(loadedSurface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/CHIP8-PS2-BACKGROUND.png", IMG_GetError());

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    //Texture properties
    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = 640;
    backgroundRect.h = 480;

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

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_RenderPresent(renderer);
    }

    return 0;
}

// End func main