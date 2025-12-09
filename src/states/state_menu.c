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
#include "settings.h"
#include "music.h"
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

SDL_Surface* settings_surface = NULL;
SDL_Texture* settings_texture = NULL;
SDL_Rect settings_rect;

SDL_Surface* skins_surface = NULL;
SDL_Texture* skins_texture = NULL;
SDL_Rect skins_rect;

extern Settings game_settings;

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

    /*SETTINGS*/
    settings_surface = TTF_RenderText_Solid(font, "Settings", (SDL_Color){255, 0, 0, 255});
    settings_texture = SDL_CreateTextureFromSurface(renderer, settings_surface);
    SDL_FreeSurface(settings_surface);

    int settingsW = 0, settingsH = 0;
    SDL_QueryTexture(settings_texture, NULL, NULL, &settingsW, &settingsH);
    settings_rect.x = (640 - settingsW) / 2;
    settings_rect.y = 300;
    settings_rect.w = settingsW;
    settings_rect.h = settingsH;

    /*SKINS TEXT*/
    skins_surface = TTF_RenderText_Solid(font, "Skins", (SDL_Color){255, 0, 0, 255});
    skins_texture = SDL_CreateTextureFromSurface(renderer, skins_surface);
    SDL_FreeSurface(skins_surface);

    int skinsW = 0, skinsH = 0;
    SDL_QueryTexture(skins_texture, NULL, NULL, &skinsW, &skinsH);
    skins_rect.x = (640 - skinsW) / 2;
    skins_rect.y = 350;
    skins_rect.w = skinsW;
    skins_rect.h = skinsH;

    /*QUIT TEXT*/
    quit_surface = TTF_RenderText_Solid(font, "Quit", (SDL_Color){255, 0, 0, 255});
    quit_texture = SDL_CreateTextureFromSurface(renderer, quit_surface);

    SDL_FreeSurface(quit_surface);

    int quitW = 0, quitH = 0;
    SDL_QueryTexture(quit_texture, NULL, NULL, &quitW, &quitH);
    quit_rect.x = (640 - quitW) / 2;
    quit_rect.y = 400;
    quit_rect.w = quitW;
    quit_rect.h = quitH;

    char TEXT_VERSION[64];
    snprintf(TEXT_VERSION, sizeof(TEXT_VERSION), "Version %d.%d.%d Pre-Alpha", GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_PATCH);

    /*VERSION*/
    version_surface = TTF_RenderText_Solid(font, TEXT_VERSION, (SDL_Color){255, 255, 255, 255});
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

int State_Config(){

    char volumen_text_in_menu[64];
    snprintf(volumen_text_in_menu, sizeof(volumen_text_in_menu), "Volumen de la musica del menu: %d", game_settings.volume_music_in_menu);

    /*VOLUMEN MUSIC MENU TEXT*/
    SDL_Surface* volumen_text_menu = TTF_RenderText_Solid(font, volumen_text_in_menu, (SDL_Color){255, 255, 255, 255});
    SDL_Texture* volumen_text_menu_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_menu);
    SDL_FreeSurface(volumen_text_menu);

    SDL_Rect volumen_text_menu_rect;
    int volumen_text_menu_W = 0, volumen_text_menu_H = 0;
    SDL_QueryTexture(volumen_text_menu_texture, NULL, NULL, &volumen_text_menu_W, &volumen_text_menu_H);
    volumen_text_menu_rect.x = (640 - volumen_text_menu_W) / 2;
    volumen_text_menu_rect.y = (480 - volumen_text_menu_H) / 2;
    volumen_text_menu_rect.w = volumen_text_menu_W;
    volumen_text_menu_rect.h = volumen_text_menu_H;

    /*VOLUMEN GAME TEXT*/
    char volumen_text_in_game[64];
    snprintf(volumen_text_in_game, sizeof(volumen_text_in_game), "Volumen de la musica del juego: %d", game_settings.volume_music_in_game);

    SDL_Surface* volumen_text_game = TTF_RenderText_Solid(font, volumen_text_in_game, (SDL_Color){255, 255, 255, 255});
    SDL_Texture* volumen_text_game_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_game);
    SDL_FreeSurface(volumen_text_game);

    SDL_Rect volumen_text_game_rect;
    int volumen_text_game_W = 0, volumen_text_game_H = 0;
    SDL_QueryTexture(volumen_text_game_texture, NULL, NULL, &volumen_text_game_W, &volumen_text_game_H);
    volumen_text_game_rect.x = (640 - volumen_text_game_W) / 2;
    volumen_text_game_rect.y = volumen_text_menu_rect.y - volumen_text_game_H - 20;
    volumen_text_game_rect.w = volumen_text_game_W;
    volumen_text_game_rect.h = volumen_text_game_H;

    /*+(Volumen menu)*/
    SDL_Surface* plus_surface = TTF_RenderText_Solid(font, "+", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* plus_texture = SDL_CreateTextureFromSurface(renderer, plus_surface);
    SDL_FreeSurface(plus_surface);

    SDL_Rect plus_rect;
    int plusW = 0, plusH = 0;
    SDL_QueryTexture(plus_texture, NULL, NULL, &plusW, &plusH);
    plus_rect.x = volumen_text_menu_rect.x + volumen_text_menu_rect.w + 10;
    plus_rect.y = volumen_text_menu_rect.y;
    plus_rect.w = plusW;
    plus_rect.h = plusH;

    /*-(Volumen menu)*/
    SDL_Surface* minus_surface = TTF_RenderText_Solid(font, "-", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* minus_texture = SDL_CreateTextureFromSurface(renderer, minus_surface);
    SDL_FreeSurface(minus_surface);

    SDL_Rect minus_rect;
    int minusW = 0, minusH = 0;
    SDL_QueryTexture(minus_texture, NULL, NULL, &minusW, &minusH);
    minus_rect.x = plus_rect.x + plus_rect.w + 10;
    minus_rect.y = volumen_text_menu_rect.y;
    minus_rect.w = minusW;
    minus_rect.h = minusH;

    /*SAVE SETTINGS TEXT*/
    SDL_Surface* save_settings_surface = TTF_RenderText_Solid(font, "Save Settings", (SDL_Color){0, 255, 0, 255});
    SDL_Texture* save_settings_texture = SDL_CreateTextureFromSurface(renderer, save_settings_surface);
    SDL_FreeSurface(save_settings_surface);

    SDL_Rect save_settings_rect;
    int save_settingsW = 0, save_settingsH = 0;
    SDL_QueryTexture(save_settings_texture, NULL, NULL, &save_settingsW, &save_settingsH);
    save_settings_rect.x = (640 - save_settingsW) / 2;
    save_settings_rect.y = minus_rect.y + minus_rect.h + 20;
    save_settings_rect.w = save_settingsW;
    save_settings_rect.h = save_settingsH;

    /*The settings were saved successfully TEXT*/
    SDL_Surface* settings_saved_surface = TTF_RenderText_Solid(font, "Settings saved successfully!", (SDL_Color){0, 255, 0, 255});
    SDL_Texture* settings_saved_texture = SDL_CreateTextureFromSurface(renderer, settings_saved_surface);
    SDL_FreeSurface(settings_saved_surface);

    SDL_Rect settings_saved_rect;
    int settings_savedW = 0, settings_savedH = 0;
    SDL_QueryTexture(settings_saved_texture, NULL, NULL, &settings_savedW, &settings_savedH);
    settings_saved_rect.x = (640 - settings_savedW) / 2;
    settings_saved_rect.y = save_settings_rect.y + save_settings_rect.h + 20;
    settings_saved_rect.w = settings_savedW;
    settings_saved_rect.h = settings_savedH;

    /*ARROW*/
    SDL_Surface* arrow = TTF_RenderText_Solid(font, "<-", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* arrow_texture = SDL_CreateTextureFromSurface(renderer, arrow);
    SDL_FreeSurface(arrow);

    SDL_Rect arrow_rect;
    int arrowW = 0, arrowH = 0;
    SDL_QueryTexture(arrow_texture, NULL, NULL, &arrowW, &arrowH);
    arrow_rect.x = 10;
    arrow_rect.y = 10;
    arrow_rect.w = arrowW;
    arrow_rect.h = arrowH;

    /*+(Volumen in game)*/
    SDL_Surface* plus_game_surface = TTF_RenderText_Solid(font, "+", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* plus_game_texture = SDL_CreateTextureFromSurface(renderer, plus_game_surface);
    SDL_FreeSurface(plus_game_surface);

    SDL_Rect plus_game_rect;
    int plus_gameW = 0, plus_gameH = 0;
    SDL_QueryTexture(plus_game_texture, NULL, NULL, &plus_gameW, &plus_gameH);
    plus_game_rect.x = volumen_text_game_rect.x + volumen_text_game_rect.w + 10;
    plus_game_rect.y = volumen_text_game_rect.y;
    plus_game_rect.h = plus_gameH;
    plus_game_rect.w = plus_gameW;

    /*-(Volumen in game)*/
    SDL_Surface* minus_game_surface = TTF_RenderText_Solid(font, "-", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* minus_game_texture = SDL_CreateTextureFromSurface(renderer, minus_game_surface);
    SDL_FreeSurface(minus_game_surface);

    SDL_Rect minus_game_rect;
    int minus_gameW = 0, minus_gameH = 0;
    SDL_QueryTexture(minus_game_texture, NULL, NULL, &minus_gameW, &minus_gameH);
    minus_game_rect.x = plus_game_rect.x + plus_game_rect.w + 10;
    minus_game_rect.y = volumen_text_game_rect.y;
    minus_game_rect.w = minus_gameW;
    minus_game_rect.h = minus_gameH;

    while(1)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_DestroyTexture(volumen_text_menu_texture);
                SDL_DestroyTexture(plus_texture);
                SDL_DestroyTexture(arrow_texture);

                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= arrow_rect.x && mx <= arrow_rect.x + arrow_rect.w &&
                   my >= arrow_rect.y && my <= arrow_rect.y + arrow_rect.h)
                {
                    SDL_DestroyTexture(volumen_text_menu_texture);
                    SDL_DestroyTexture(plus_texture);
                    SDL_DestroyTexture(arrow_texture);

                    Init_State_Menu();
                    Update_State_Menu();
                    return 0;
                }

                if(mx >= plus_rect.x && mx <= plus_rect.x + plus_rect.w &&
                   my >= plus_rect.y && my <= plus_rect.y + plus_rect.h)
                {
                    if(game_settings.volume_music_in_menu < 100){
                        game_settings.volume_music_in_menu += 10;
                        if(game_settings.volume_music_in_menu > 100){
                            game_settings.volume_music_in_menu = 100;
                        }
                    }

                    ChangeMusicVolume(game_settings.volume_music_in_menu);

                    SDL_DestroyTexture(volumen_text_menu_texture);

                    char volumen_text_in_menu[64];
                    snprintf(volumen_text_in_menu, sizeof(volumen_text_in_menu), "Volumen de la musica del menu: %d", game_settings.volume_music_in_menu);

                    /*CLEAN UP AND RECREATE TEXTURE*/
                    SDL_Surface* volumen_text_menu = TTF_RenderText_Solid(font, volumen_text_in_menu, (SDL_Color){255, 255, 255, 255});
                    volumen_text_menu_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_menu);
                    SDL_FreeSurface(volumen_text_menu);
                }

                if(mx >= minus_rect.x && mx <= minus_rect.x + minus_rect.w &&
                   my >= minus_rect.y && my <= minus_rect.y + minus_rect.h)
                {
                    if(game_settings.volume_music_in_menu > 0){
                        game_settings.volume_music_in_menu -= 10;
                        if(game_settings.volume_music_in_menu < 0){
                            game_settings.volume_music_in_menu = 0;
                        }
                    }
                    ChangeMusicVolume(game_settings.volume_music_in_menu);

                    SDL_DestroyTexture(volumen_text_menu_texture);

                    char volumen_text_in_menu[64];
                    snprintf(volumen_text_in_menu, sizeof(volumen_text_in_menu), "Volumen de la musica del menu: %d", game_settings.volume_music_in_menu);

                    /*CLEAN UP AND RECREATE TEXTURE*/
                    SDL_Surface* volumen_text_menu = TTF_RenderText_Solid(font, volumen_text_in_menu, (SDL_Color){255, 255, 255, 255});
                    volumen_text_menu_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_menu);
                    SDL_FreeSurface(volumen_text_menu);
                }

                if(mx >= plus_game_rect.x && mx <= plus_game_rect.x + plus_game_rect.w &&
                   my >= plus_game_rect.y && my <= plus_game_rect.y + plus_game_rect.h)
                {
                    if(game_settings.volume_music_in_game < 100){
                        game_settings.volume_music_in_game += 10;
                        if(game_settings.volume_music_in_game > 100){
                            game_settings.volume_music_in_game = 100;
                        }
                    }

                    SDL_DestroyTexture(volumen_text_game_texture);

                    char volumen_text_in_game[64];
                    snprintf(volumen_text_in_game, sizeof(volumen_text_in_game), "Volumen de la musica del juego: %d", game_settings.volume_music_in_game);

                    /*CLEAN UP AND RECREATE TEXTURE*/
                    SDL_Surface* volumen_text_game = TTF_RenderText_Solid(font, volumen_text_in_game, (SDL_Color){255, 255, 255, 255});
                    volumen_text_game_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_game);
                    SDL_FreeSurface(volumen_text_game);
                }

                if(mx >= minus_game_rect.x && mx <= minus_game_rect.x + minus_game_rect.w &&
                   my >= minus_game_rect.y && my <= minus_game_rect.y + minus_game_rect.h)
                {
                    if(game_settings.volume_music_in_game > 0){
                        game_settings.volume_music_in_game -= 10;
                        if(game_settings.volume_music_in_game < 0){
                            game_settings.volume_music_in_game = 0;
                        }
                    }

                    SDL_DestroyTexture(volumen_text_game_texture);

                    char volumen_text_in_game[64];
                    snprintf(volumen_text_in_game, sizeof(volumen_text_in_game), "Volumen de la musica del juego: %d", game_settings.volume_music_in_game);

                    /*CLEAN UP AND RECREATE TEXTURE*/
                    SDL_Surface* volumen_text_game = TTF_RenderText_Solid(font, volumen_text_in_game, (SDL_Color){255, 255, 255, 255});
                    volumen_text_game_texture = SDL_CreateTextureFromSurface(renderer, volumen_text_game);
                    SDL_FreeSurface(volumen_text_game);
                }

                if(mx >= save_settings_rect.x && mx <= save_settings_rect.x + save_settings_rect.w &&
                   my >= save_settings_rect.y && my <= save_settings_rect.y + save_settings_rect.h)
                {
                    SaveSettings(&game_settings);
                    SDL_RenderCopy(renderer, settings_saved_texture, NULL, &settings_saved_rect);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000); //Show message for 1 seconds
                    SDL_DestroyTexture(settings_saved_texture);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, volumen_text_menu_texture, NULL, &volumen_text_menu_rect); /*VOLUMEN TEXT MENU*/
        SDL_RenderCopy(renderer, plus_texture, NULL, &plus_rect); /*+*/
        SDL_RenderCopy(renderer, minus_texture, NULL, &minus_rect); /*-*/
        SDL_RenderCopy(renderer, volumen_text_game_texture, NULL, &volumen_text_game_rect); /*VOLUMEN GAME TEXT*/
        SDL_RenderCopy(renderer, plus_game_texture, NULL, &plus_game_rect); /*+ GAME*/
        SDL_RenderCopy(renderer, minus_game_texture, NULL, &minus_game_rect); /*- GAME*/
        SDL_RenderCopy(renderer, save_settings_texture, NULL, &save_settings_rect); /*SAVE SETTINGS TEXT*/
        SDL_RenderCopy(renderer, arrow_texture, NULL, &arrow_rect); /*ARROW*/

        SDL_RenderPresent(renderer);
    }

    return 0;
}

int State_Skins(){

    /*HUEHUEHUEHUE XDDDDDDDD*/
    SDL_Surface* huehuehue_surface = IMG_Load("skins/huehuehue.png");
    SDL_Texture* huehuehue_texture = SDL_CreateTextureFromSurface(renderer, huehuehue_surface);
    SDL_FreeSurface(huehuehue_surface);

    SDL_Rect huehuehue_rect;
    int huehuehueW = 0, huehuehueH = 0;
    SDL_QueryTexture(huehuehue_texture, NULL, NULL, &huehuehueW, &huehuehueH);
    huehuehue_rect.x = 640 / 2 - (huehuehueW / 4) / 2;
    huehuehue_rect.y = 100;
    huehuehue_rect.w = huehuehueW / 4;
    huehuehue_rect.h = huehuehueH / 4;

    /*DEFAULT SKIN PLAYER PREVIEW*/
    SDL_Surface* default_skin_surface = IMG_Load("sprites/gif/idle_player.gif");
    SDL_Texture* default_skin_texture = SDL_CreateTextureFromSurface(renderer, default_skin_surface);
    SDL_FreeSurface(default_skin_surface);

    SDL_Rect default_skin_rect;
    int default_skinW = 0, default_skinH = 0;
    SDL_QueryTexture(default_skin_texture, NULL, NULL, &default_skinW, &default_skinH);
    default_skin_rect.x = 640 / 2 - (default_skinW / 4) / 2;
    default_skin_rect.y = 100;
    default_skin_rect.w = default_skinW;
    default_skin_rect.h = default_skinH;

    /*SKINS TEXT*/
    SDL_Surface* skins_text_surface = TTF_RenderText_Solid(font, "Skins", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* skins_text_texture = SDL_CreateTextureFromSurface(renderer, skins_text_surface);
    SDL_FreeSurface(skins_text_surface);

    SDL_Rect skins_text_rect;
    int skins_textW = 0, skins_textH = 0;
    SDL_QueryTexture(skins_text_texture, NULL, NULL, &skins_textW, &skins_textH);
    skins_text_rect.x = (640 - skins_textW) / 2;
    skins_text_rect.y = 50;
    skins_text_rect.w = skins_textW;
    skins_text_rect.h = skins_textH;

    /*<-*/
    SDL_Surface* arrow = TTF_RenderText_Solid(font, "<-", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* arrow_texture = SDL_CreateTextureFromSurface(renderer, arrow);
    SDL_FreeSurface(arrow);

    SDL_Rect arrow_rect;
    int arrowW = 0, arrowH = 0;
    SDL_QueryTexture(arrow_texture, NULL, NULL, &arrowW, &arrowH);
    arrow_rect.x = 10;
    arrow_rect.y = 10;
    arrow_rect.w = arrowW;
    arrow_rect.h = arrowH;

    while(1)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_DestroyTexture(skins_text_texture);

                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }

            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        /*/
                        SDL_DestroyTexture(huehuehue_texture);
                        SDL_RenderCopy(renderer, default_skin_texture, NULL, &default_skin_rect); /*DEFAULT SKIN PREVIEW
                        SDL_RenderPresent(renderer);
                        break;
                        /*/
                    case SDLK_RIGHT:

                        /*/SDL_DestroyTexture(default_skin_texture);
                        default_skin_surface = IMG_Load("sprites/gif/idle_player.gif");
                        default_skin_texture = SDL_CreateTextureFromSurface(renderer, default_skin_surface);
                        SDL_FreeSurface(default_skin_surface);
                        SDL_RenderCopy(renderer, huehuehue_texture, NULL, &huehuehue_rect); /*HUEHUEHUE SKIN PREVIEW
                        SDL_RenderPresent(renderer);
                        /*/
                        break;
                    default:
                        break;
                }
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= arrow_rect.x && mx <= arrow_rect.x + arrow_rect.w &&
                   my >= arrow_rect.y && my <= arrow_rect.y + arrow_rect.h)
                {
                    SDL_DestroyTexture(skins_text_texture);
                    SDL_DestroyTexture(huehuehue_texture);
                    SDL_DestroyTexture(arrow_texture);

                    Init_State_Menu();
                    Update_State_Menu();
                    return 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, skins_text_texture, NULL, &skins_text_rect); /*SKINS TEXT*/
        SDL_RenderCopy(renderer, arrow_texture, NULL, &arrow_rect); /*ARROW*/
        SDL_RenderCopy(renderer, default_skin_texture, NULL, &default_skin_rect); /*DEFAULT SKIN PREVIEW*/

        SDL_RenderPresent(renderer);
    }

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
                    SDL_DestroyTexture(start_texture);
                    SDL_DestroyTexture(quit_texture);
                    SDL_DestroyTexture(version_texture);
                    SDL_DestroyTexture(settings_texture);
                    TTF_CloseFont(font);
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                }else if(mx >= settings_rect.x && mx <= settings_rect.x + settings_rect.w &&
                   my >= settings_rect.y && my <= settings_rect.y + settings_rect.h)
                {
                    SDL_DestroyTexture(start_texture);
                    SDL_DestroyTexture(quit_texture);
                    SDL_DestroyTexture(version_texture);
                    SDL_DestroyTexture(settings_texture);

                    State_Config();
                }else if(mx >= skins_rect.x && mx <= skins_rect.x + skins_rect.w &&
                   my >= skins_rect.y && my <= skins_rect.y + skins_rect.h)
                {
                    SDL_DestroyTexture(start_texture);
                    SDL_DestroyTexture(quit_texture);
                    SDL_DestroyTexture(version_texture);
                    SDL_DestroyTexture(settings_texture);
                    SDL_DestroyTexture(skins_texture);

                    State_Skins();
                }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, start_texture, NULL, &start_rect); //START TEXT
        SDL_RenderCopy(renderer, quit_texture, NULL, &quit_rect); //QUIT TEXT
        SDL_RenderCopy(renderer, skins_texture, NULL, &skins_rect); //SKINS TEXT
        SDL_RenderCopy(renderer, version_texture, NULL, &version_rect); //VERSION TEXT
        SDL_RenderCopy(renderer, settings_texture, NULL, &settings_rect); //SETTINGS TEXT

        SDL_RenderPresent(renderer);
        }
    }

    return 0;
}