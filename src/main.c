/*
THE MUTANTS

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka ?)
File: main.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2024
*/

#include <stdio.h>
#include <SDL2/SDL.h>

//Func Main
int main()
{   
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    /*Initialize SDL*/
    if(SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    /*Create window*/
    window = SDL_CreateWindow("The Mutant's", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN); //Window 640x480 fullscreen
    if(window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    /*Create renderer for window*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

    SDL_ShowCursor(SDL_DISABLE); /*Hide cursor(I'll be using this in the game soon, 
                                 * I'll remove it from here and move it when I add a main menu :) )
                                 */
    while(1) //Main loop
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
            }
        }

        /*TESTING*/
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); //Red
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}

// End func main