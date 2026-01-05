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
#include "update_system.h"
#include "show_notifications.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>

char download_path[MAX_PATH];

/* Pi constant for spinner math */
static const double PI = 3.14159265358979323846;

#include "player.h"
extern IDLE_PLAYER idle_player;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

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

SDL_Surface* Check_for_updates_surface = NULL;
SDL_Texture* Check_for_updates_texture = NULL;
SDL_Rect Check_for_updates_rect;

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

    /*CHECK FOR UPDATES TEXT*/
    Check_for_updates_surface = TTF_RenderText_Solid(font, "Check for updates", (SDL_Color){255, 0, 0, 255});
    Check_for_updates_texture = SDL_CreateTextureFromSurface(renderer, Check_for_updates_surface);
    SDL_FreeSurface(Check_for_updates_surface);

    int Check_for_updatesW = 0, Check_for_updatesH = 0;
    SDL_QueryTexture(Check_for_updates_texture, NULL, NULL, &Check_for_updatesW, &Check_for_updatesH);
    Check_for_updates_rect.x = (640 - Check_for_updatesW) / 2;
    Check_for_updates_rect.y = 250;
    Check_for_updates_rect.w = Check_for_updatesW;
    Check_for_updates_rect.h = Check_for_updatesH;

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
                if(volumen_text_menu_texture){ SDL_DestroyTexture(volumen_text_menu_texture); volumen_text_menu_texture = NULL; }
                if(plus_texture){ SDL_DestroyTexture(plus_texture); plus_texture = NULL; }
                if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }

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
                    if(volumen_text_menu_texture){ SDL_DestroyTexture(volumen_text_menu_texture); volumen_text_menu_texture = NULL; }
                    if(plus_texture){ SDL_DestroyTexture(plus_texture); plus_texture = NULL; }
                    if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }

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

                    if(volumen_text_menu_texture){ SDL_DestroyTexture(volumen_text_menu_texture); volumen_text_menu_texture = NULL; }

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

                    if(volumen_text_menu_texture){ SDL_DestroyTexture(volumen_text_menu_texture); volumen_text_menu_texture = NULL; }

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

                    if(volumen_text_game_texture){ SDL_DestroyTexture(volumen_text_game_texture); volumen_text_game_texture = NULL; }

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

                    if(volumen_text_game_texture){ SDL_DestroyTexture(volumen_text_game_texture); volumen_text_game_texture = NULL; }

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
                    if(settings_saved_texture){ SDL_DestroyTexture(settings_saved_texture); settings_saved_texture = NULL; }
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

    int index_selected_skin = 1;

    /*HUEHUEHUEHUE XDDDDDDDD*/
    SDL_Surface* huehuehue_surface = IMG_Load("skins/huehuehue.png");
    SDL_Texture* huehuehue_texture = SDL_CreateTextureFromSurface(renderer, huehuehue_surface);
    SDL_FreeSurface(huehuehue_surface);

    SDL_Rect huehuehue_rect;
    int huehuehueW = 0, huehuehueH = 0;
    SDL_QueryTexture(huehuehue_texture, NULL, NULL, &huehuehueW, &huehuehueH);
    huehuehue_rect.x = 320;
    huehuehue_rect.y = 150;
    huehuehue_rect.w = huehuehueW / 6;
    huehuehue_rect.h = huehuehueH / 6;

    SDL_Surface* default_skin_surface = IMG_Load("sprites/gif/idle_player.gif");
    SDL_Texture* default_skin_texture = SDL_CreateTextureFromSurface(renderer, default_skin_surface);

    SDL_Rect default_skin_rect;
    int default_skinW = 0, default_skinH = 0;
    SDL_QueryTexture(default_skin_texture, NULL, NULL, &default_skinW, &default_skinH);

    default_skin_rect.x = 0;
    default_skin_rect.y = 100;
    default_skin_rect.w = default_skinW * 4;
    default_skin_rect.h = default_skinH * 4;

    /*GOKU SKIN*/
    SDL_Surface* goku_surface = IMG_Load("skins/goku.png");
    SDL_Texture* goku_texture = SDL_CreateTextureFromSurface(renderer, goku_surface);
    SDL_FreeSurface(goku_surface);

    SDL_Rect goku_rect;
    int gokuW = 0, gokuH = 0;
    SDL_QueryTexture(goku_texture, NULL, NULL, &gokuW, &gokuH);
    goku_rect.x = huehuehue_rect.x + 100;
    goku_rect.y = 150 - huehuehue_rect.h;
    goku_rect.w = gokuW / 50;
    goku_rect.h = gokuH / 50;

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

    /*->*/
    SDL_Surface* arrow2 = TTF_RenderText_Solid(font, "->", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* arrow2_texture = SDL_CreateTextureFromSurface(renderer, arrow2);
    SDL_FreeSurface(arrow2);

    SDL_Rect arrow2_rect;
    int arrow2W = 0, arrow2H = 0;
    SDL_QueryTexture(arrow2_texture, NULL, NULL, &arrow2W, &arrow2H);
    arrow2_rect.x = default_skin_rect.x;
    arrow2_rect.y = 150;
    arrow2_rect.w = arrow2W;
    arrow2_rect.h = arrow2H;

    while(1)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                if(skins_text_texture){ SDL_DestroyTexture(skins_text_texture); skins_text_texture = NULL; }
                if(huehuehue_texture){ SDL_DestroyTexture(huehuehue_texture); huehuehue_texture = NULL; }
                if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }

                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }

            /* Handle keyboard presses so arrow keys select and apply skins immediately */
            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    if(index_selected_skin < 3)
                    {
                        index_selected_skin++;
                        arrow2_rect.x += 280;
                        SDL_Log("State_Skins: keyboard selected skin %d (RIGHT)\n", index_selected_skin);
                        if(index_selected_skin == 1)
                        {
                            ChangePlayerSkin("sprites/idle_player_spritesheet.png", "sprites/walk_player_spritesheet.png", "sprites/jump_player-spritesheet.png");
                        }
                        else if(index_selected_skin == 2)
                        {
                            ChangePlayerSkin("sprites/IDLE-METAL-SONIC.png", "sprites/WALK-METAL-SONIC.png", "sprites/jump-METAL-SONIC.png");
                        }
                        else if(index_selected_skin == 3)
                        {
                            ChangePlayerSkin("sprites/IDLEGOKU.png", "sprites/WALKGOKU.png", "sprites/JUMPGOKU.png");
                        }
                    }
                }
                else if(e.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    if(index_selected_skin > 1)
                    {
                        index_selected_skin--;
                        arrow2_rect.x -= 280;
                        SDL_Log("State_Skins: keyboard selected skin %d (LEFT)\n", index_selected_skin);
                        if(index_selected_skin == 1)
                        {
                            ChangePlayerSkin("sprites/idle_player_spritesheet.png", "sprites/walk_player_spritesheet.png", "sprites/jump_player-spritesheet.png");
                        }
                        else if(index_selected_skin == 2)
                        {
                            ChangePlayerSkin("sprites/IDLE-METAL-SONIC.png", "sprites/WALK-METAL-SONIC.png", "sprites/jump-METAL-SONIC.png");
                        }
                    }
                }
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= arrow_rect.x && mx <= arrow_rect.x + arrow_rect.w &&
                   my >= arrow_rect.y && my <= arrow_rect.y + arrow_rect.h)
                {
                    if(skins_text_texture){ SDL_DestroyTexture(skins_text_texture); skins_text_texture = NULL; }
                    if(huehuehue_texture){ SDL_DestroyTexture(huehuehue_texture); huehuehue_texture = NULL; }
                    if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }

                    return 0;
                }

            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, arrow2_texture, NULL, &arrow2_rect);
        SDL_RenderCopy(renderer, skins_text_texture, NULL, &skins_text_rect); /*SKINS TEXT*/
        SDL_RenderCopy(renderer, default_skin_texture, NULL, &default_skin_rect); /*DEFAULT ICON*/
        SDL_RenderCopy(renderer, goku_texture, NULL, &goku_rect); /*GOKU ICON*/
        SDL_RenderCopy(renderer, arrow_texture, NULL, &arrow_rect); /*ARROW*/
        SDL_RenderCopy(renderer, huehuehue_texture, NULL, &huehuehue_rect); /*HUEHUEHUE ICON*/

        SDL_RenderPresent(renderer);
    }

}

/* Helper thread wrapper for download so UI stays responsive */
typedef struct {
    const char* url;
    const char* filename;
} DownloadArgs;

static volatile int download_result = -1;

static int download_thread(void* data)
{
    DownloadArgs* args = (DownloadArgs*)data;
    int res = download(args->url, args->filename);
    download_result = res;
    free(args);
    return res;
}

int StateUpdate()
{
    char tempFolder[MAX_PATH];
    char download_path[MAX_PATH];
    
    /* Get temp path and prepare filename outside the loop to avoid repeating work */
    GetTempPath(MAX_PATH, tempFolder);
    snprintf(download_path, sizeof(download_path), "%sUpdate.exe", tempFolder);

    /* Render "Downloading..." text once to avoid memory leaks */
    SDL_Surface* dl_surface = TTF_RenderText_Solid(font, "Downloading update...", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* dl_texture = SDL_CreateTextureFromSurface(renderer, dl_surface);
    SDL_FreeSurface(dl_surface);

    SDL_Rect dl_rect;
    int dlW = 0, dlH = 0;
    SDL_QueryTexture(dl_texture, NULL, NULL, &dlW, &dlH);
    dl_rect.x = (640 - dlW) / 2;
    dl_rect.y = 220;
    dl_rect.w = dlW;
    dl_rect.h = dlH;

    /* start download in background thread - MOVED OUTSIDE OF LOOP to prevent infinite threads */
    if(!start_texture && !quit_texture && !version_texture && !settings_texture && !skins_texture && !Check_for_updates_texture)
    {
        DownloadArgs* args = (DownloadArgs*)malloc(sizeof(DownloadArgs));
        args->url = "https://the-mutants-updates.web.app/update";
        args->filename = strdup(download_path); // Use strdup to ensure string safety
        download_result = -1;
        
        SDL_Thread* thr = SDL_CreateThread(download_thread, "downloader", (void*)args);
        if(thr == NULL)
        {
            /* fallback to synchronous download if thread creation fails */
            download_result = download(args->url, args->filename);
            free(args);
        }
    }

    /* Wait for download to finish while keeping UI responsive
     * and render percentage + animated dots
     */
    while(download_result == -1)
    {
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_QUIT)
            {
                SDL_DestroyTexture(dl_texture);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }
        }

        /* compute percentage */
        int percent = 0;
        curl_off_t total = download_total;
        curl_off_t done = download_bytes;
        if(total > 0) percent = (int)((done * 100) / total);

        /* render downloading + percentage */
        char percent_text[64];
        if(total > 0)
            snprintf(percent_text, sizeof(percent_text), "%d%%", percent);
        else
            snprintf(percent_text, sizeof(percent_text), "--%%");

        SDL_Surface* percent_surf = TTF_RenderText_Solid(font, percent_text, (SDL_Color){255,255,255,255});
        SDL_Texture* percent_tex = SDL_CreateTextureFromSurface(renderer, percent_surf);
        SDL_FreeSurface(percent_surf);

        int pW=0,pH=0;
        SDL_QueryTexture(percent_tex, NULL, NULL, &pW, &pH);
        SDL_Rect pRect = { (640 - pW)/2, 260, pW, pH };

        /* animate dots (4 dots around) */
        Uint32 t = SDL_GetTicks();
        int frame = (t / 250) % 8; /* 8 positions */
        int dotRadius = 4;
        int cx = 320; int cy = 320; int r = 14;

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, dl_texture, NULL, &dl_rect);
        SDL_RenderCopy(renderer, percent_tex, NULL, &pRect);

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        for(int i=0;i<8;i++)
        {
            double ang = (i * M_PI * 2) / 8.0;
            int dx = (int)(cx + cos(ang) * r);
            int dy = (int)(cy + sin(ang) * r);
            SDL_Rect dot = { dx - dotRadius, dy - dotRadius, dotRadius*2, dotRadius*2 };
            
            /* fade inactive dots */
            if(((i + frame) % 8) < 3)
            {
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            } else {
                SDL_SetRenderDrawColor(renderer, 180,180,180,255);
            }
            SDL_RenderFillRect(renderer, &dot);
        }

        SDL_RenderPresent(renderer);

        /* CRITICAL: Destroy percentage texture every frame to prevent memory leak */
        SDL_DestroyTexture(percent_tex);
        
        SDL_Delay(16); // Prevent 100% CPU usage
    }

    /* Destroy main texture after loop ends */
    SDL_DestroyTexture(dl_texture);

    /* Launch installer and exit */
    #ifdef _WIN32
    if(download_result == 0){
        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.lpFile = download_path;
        sei.nShow = SW_SHOWNORMAL;

        ShowWindowsNotification(window, "The Mutant's updater", "The setup will start now. The game will close.");

        if(ShellExecuteEx(&sei)){
            MoveFileEx(download_path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_Quit();
            exit(0);
        }
    }
    #endif

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
                if(start_texture){ SDL_DestroyTexture(start_texture); start_texture = NULL; }
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
                    if(start_texture){ SDL_DestroyTexture(start_texture); start_texture = NULL; }
                    if(quit_texture){ SDL_DestroyTexture(quit_texture); quit_texture = NULL; }
                    if(version_texture){ SDL_DestroyTexture(version_texture); version_texture = NULL; }
                    if(settings_texture){ SDL_DestroyTexture(settings_texture); settings_texture = NULL; }
                    TTF_CloseFont(font);
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                }else if(mx >= settings_rect.x && mx <= settings_rect.x + settings_rect.w &&
                   my >= settings_rect.y && my <= settings_rect.y + settings_rect.h)
                {
                    if(start_texture){ SDL_DestroyTexture(start_texture); start_texture = NULL; }
                    if(quit_texture){ SDL_DestroyTexture(quit_texture); quit_texture = NULL; }
                    if(version_texture){ SDL_DestroyTexture(version_texture); version_texture = NULL; }
                    if(settings_texture){ SDL_DestroyTexture(settings_texture); settings_texture = NULL; }

                    State_Config();
                    /* Recreate menu textures after returning from config */
                    Init_State_Menu();
                }else if(mx >= skins_rect.x && mx <= skins_rect.x + skins_rect.w &&
                   my >= skins_rect.y && my <= skins_rect.y + skins_rect.h)
                {
                    if(start_texture){ SDL_DestroyTexture(start_texture); start_texture = NULL; }
                    if(quit_texture){ SDL_DestroyTexture(quit_texture); quit_texture = NULL; }
                    if(version_texture){ SDL_DestroyTexture(version_texture); version_texture = NULL; }
                    if(settings_texture){ SDL_DestroyTexture(settings_texture); settings_texture = NULL; }
                    if(skins_texture){ SDL_DestroyTexture(skins_texture); skins_texture = NULL; }
                    if(Check_for_updates_texture){ SDL_DestroyTexture(Check_for_updates_texture); Check_for_updates_texture = NULL; }

                    State_Skins();
                    /* Recreate menu textures after returning from skins */
                    Init_State_Menu();
                }else if(mx >= Check_for_updates_rect.x && mx <= Check_for_updates_rect.x + Check_for_updates_rect.w &&
                my>= Check_for_updates_rect.y && my <= Check_for_updates_rect.y + Check_for_updates_rect.h)
                {
                    if(start_texture){ SDL_DestroyTexture(start_texture); start_texture = NULL; }
                    if(quit_texture){ SDL_DestroyTexture(quit_texture); quit_texture = NULL; }
                    if(version_texture){ SDL_DestroyTexture(version_texture); version_texture = NULL; }
                    if(settings_texture){ SDL_DestroyTexture(settings_texture); settings_texture = NULL; }
                    if(skins_texture){ SDL_DestroyTexture(skins_texture); skins_texture = NULL; }
                    if(Check_for_updates_texture){ SDL_DestroyTexture(Check_for_updates_texture); Check_for_updates_texture = NULL; }

                    StateUpdate();
                }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, start_texture, NULL, &start_rect); //START TEXT
        SDL_RenderCopy(renderer, quit_texture, NULL, &quit_rect); //QUIT TEXT
        SDL_RenderCopy(renderer, Check_for_updates_texture, NULL, &Check_for_updates_rect); //CHECK FOR UPDATES TEXT
        SDL_RenderCopy(renderer, skins_texture, NULL, &skins_rect); //SKINS TEXT
        SDL_RenderCopy(renderer, version_texture, NULL, &version_rect); //VERSION TEXT
        SDL_RenderCopy(renderer, settings_texture, NULL, &settings_rect); //SETTINGS TEXT

        SDL_RenderPresent(renderer);
        }
    }

    return 0;
}