/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: main.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/
/* pitos*/ 

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include "settings.h"
#include "music.h"
#include "states.h"
#include "version.h"
#include "update_system.h"

#ifdef __MINGW32__
#define main SDL_main 
#endif

#define BASE_WIDTH 640
#define BASE_HEIGHT 480

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Settings game_settings;

int InitSDLAndSubSystems()
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

    if(TTF_Init() < 0){
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    char window_title[64];
    snprintf(window_title, sizeof(window_title), "The Mutant's %d.%d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_PATCH);

    /*Create window*/
    window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); //Window 640x480
    if(window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

    /*Create renderer for window*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

    SDL_RenderSetLogicalSize(renderer, BASE_WIDTH, BASE_HEIGHT); //Set logical size for renderer

    return 1;
}

//Func Main
int main(int argc, char* argv[])
{   
    #ifdef _WIN32
    // Redirigir stdout/stderr a la consola de VS
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    #endif

    //download("https://download1075.mediafire.com/1k3jzovrw69gi-N8n2cbG_41_axoqZ0TjAB6gPk6Bo4YyC25DgDN5nlI6p9K4p61PeBKh4IAYA2m-EN2S1MEEClPiAmkiLq_LW6mcyZQlpnBZBac1ZzobgEvZrtn7Iq0Ds0Q7NafMrDY3BPPfN-O8KRnVZrK99QdZwPEjEEEUZ4qTnd-/zvlrd6lakmq17uy/The_Mutants.zip", "old.zip"); //TESTING WITH THE MUTANTS OLD VERSION :)

    InitSDLAndSubSystems(); //Initialize SDL and subsystems

    InitMusic(); //Initialize music subsystem
    LoadSettingsFromFile(&game_settings); //Load settings from file

    PlayMusicStateMenu(); //Play music for menu state

    /*STATES*/
    Init_State_Menu(); //Initialize menu state
    Update_State_Menu(); //Update menu state

    /*
    Cuando el jugador presiona "Start" en el menú, se sale del bucle del menú y se inicia el estado del juego.
    */

    PlayMusicStateGame(); //Play music for game state

    Init_State_Game(); //Initialize game state
    Update_State_Game(); //Update game state

    return 0;
}

// End func main
