/*
Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka ?)
File: main.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.
*/

#include <stdio.h>
#include <SDL2/SDL.h>

//Func Main
int main()
{   
    SDL_Window* window = NULL;

    /*Initialize SDL*/
    if(SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    /*Create window*/
    window = SDL_CreateWindow("The Mutant's", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if(window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    while(1)
    {
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) /*EXIT*/
                break;
        }
    }

    return 0;
}