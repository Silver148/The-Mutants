/*
THE MUTANT'S

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
#include <stdbool.h>
#include "states.h"

#ifdef __MINGW32__
#define main SDL_main 
#endif

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Func Main
int main(int argc, char* argv[])
{   
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

    Init_State_Game(); //Initialize game state

    Update_State_Game(); //Update game state

    return 0;
}

// End func main
