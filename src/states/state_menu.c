/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: state_menu.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/*
State Menu :D
*/

#include "states.h"
#include "global_vars.h"
#include "version.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Renderer* renderer;

SDL_Surface* start_surface = NULL;
SDL_Texture* start_texture = NULL;
SDL_Rect start_rect;

TTF_Font* font = NULL;

SDL_Rect quit_rect;
SDL_Surface* quit_surface = NULL;
SDL_Texture* quit_texture = NULL;

SDL_Surface* version_surface = NULL;
SDL_Texture* version_texture = NULL;
SDL_Rect version_rect;

int Init_State_Menu()
{
    font = TTF_OpenFont("fonts/SYSTEMIA.ttf", 24); //Cargar fuente para el menú

    /*¨START TEXT*/
    start_surface = TTF_RenderText_Solid(font, "Start", (SDL_Color){255, 0, 0, 255});
    start_texture = SDL_CreateTextureFromSurface(renderer, start_surface);

    SDL_FreeSurface(start_surface);

    int startW = 0, startH = 0;
    SDL_QueryTexture(start_texture, NULL, NULL, &startW, &startH);
    start_rect.x = (640 - startW) / 2;
    start_rect.y = 200;
    start_rect.w = startW;
    start_rect.h = startH;

    /*QUIT TEXT*/
    quit_surface = TTF_RenderText_Solid(font, "Quit", (SDL_Color){255, 0, 0, 255});
    quit_texture = SDL_CreateTextureFromSurface(renderer, quit_surface);

    SDL_FreeSurface(quit_surface);

    int quitW = 0, quitH = 0;
    SDL_QueryTexture(quit_texture, NULL, NULL, &quitW, &quitH);
    quit_rect.x = (640 - quitW) / 2;
    quit_rect.y = 300;
    quit_rect.w = quitW;
    quit_rect.h = quitH;

    /*VERSION*/
    version_surface = TTF_RenderText_Solid(font, "Version 0.19 Pre-Alpha", (SDL_Color){255, 255, 255, 255});
    version_texture = SDL_CreateTextureFromSurface(renderer, version_surface);

    SDL_FreeSurface(version_surface);

    int versionW = 0, versionH = 0;
    SDL_QueryTexture(version_texture, NULL, NULL, &versionW, &versionH);
    version_rect.x = 10;
    version_rect.y = 10;
    version_rect.w = versionW;
    version_rect.h = versionH;

    return 0;
}

int Update_State_Menu()
{

    while(1) //Main loop(TESTING)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_DestroyTexture(start_texture);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= start_rect.x && mx <= start_rect.x + start_rect.w &&
                   my >= start_rect.y && my <= start_rect.y + start_rect.h)
                {
                    return 1;
                }else if(mx >= quit_rect.x && mx <= quit_rect.x + quit_rect.w &&
                   my >= quit_rect.y && my <= quit_rect.y + quit_rect.h)
                {
                    SDL_Quit();
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, start_texture, NULL, &start_rect); //START TEXT
        SDL_RenderCopy(renderer, quit_texture, NULL, &quit_rect); //QUIT TEXT
        SDL_RenderCopy(renderer, version_texture, NULL, &version_rect); //VERSION TEXT

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(start_texture);
    SDL_DestroyTexture(quit_texture);
    TTF_Quit();

    return 0;
}