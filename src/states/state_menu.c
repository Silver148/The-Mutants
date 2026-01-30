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
#include <string.h>

char download_path[MAX_PATH];

/* Pi constant for spinner math */
static const double PI = 3.14159265358979323846;

/* Draw a simple vertical gradient from top color to bottom color as a fallback background */
static void DrawVerticalGradient(SDL_Renderer* rend, SDL_Color top, SDL_Color bottom)
{
    int w = 640;
    int h = 480;
    for(int y = 0; y < h; y++) {
        float t = (float)y / (float)(h - 1);
        Uint8 r = (Uint8)((1.0f - t) * top.r + t * bottom.r);
        Uint8 g = (Uint8)((1.0f - t) * top.g + t * bottom.g);
        Uint8 b = (Uint8)((1.0f - t) * top.b + t * bottom.b);
        SDL_SetRenderDrawColor(rend, r, g, b, 255);
        SDL_Rect row = {0, y, w, 1};
        SDL_RenderFillRect(rend, &row);
    }
}

#include "player.h"
extern IDLE_PLAYER idle_player;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/* Forward declaration so CreateTextTextureWithGlyphs can be used before its definition */
static SDL_Texture* CreateTextTextureWithGlyphs(const char* text, TTF_Font* font, SDL_Color color, int gap_adjust, SDL_Rect* out_rect);
static SDL_Texture* CreateOutlinedTextTexture(const char* text, TTF_Font* font, SDL_Color fg_color, SDL_Color outline_color, int outline_px, int gap_adjust, SDL_Rect* out_rect);

SDL_Surface* start_surface = NULL;
SDL_Texture* start_texture = NULL;
SDL_Rect start_rect;

SDL_Texture* menu_bg_texture = NULL;

SDL_Texture* start_outline_texture = NULL;
SDL_Rect start_outline_rect;
SDL_Texture* settings_outline_texture = NULL;
SDL_Rect settings_outline_rect;
SDL_Texture* skins_outline_texture = NULL;
SDL_Rect skins_outline_rect;
SDL_Texture* Check_for_updates_outline_texture = NULL;
SDL_Rect Check_for_updates_outline_rect;
SDL_Texture* quit_outline_texture = NULL;
SDL_Rect quit_outline_rect;
SDL_Texture* version_outline_texture = NULL;
SDL_Rect version_outline_rect;

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

SDL_Texture* tex_fullscreen_on = NULL;
SDL_Texture* tex_fullscreen_off = NULL;
SDL_Rect fullscreen_rect;

extern Settings game_settings;
extern STATES game_state;
extern bool state_game_ready;
extern bool state_menu_ready;

int Init_State_Menu()
{
    font = TTF_OpenFont("fonts/Half Awake.ttf", 28); //Cargar fuente para el menú (tamaño original)

    /*¨START TEXT with outline (tighter letter spacing)*/
    {
        const char* txt = "Start";
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2; /* tighten letters by 2px */
        int outline_px = 2;
        SDL_Rect out_r = {0};
        /* create combined texture (outline + main) so outline matches the glyph exactly */
        start_texture = CreateOutlinedTextTexture(txt, font, fg_col, outline_col, outline_px, gap, &out_r);
        start_rect.w = out_r.w; start_rect.h = out_r.h; start_rect.x = (640 - out_r.w) / 2; start_rect.y = 180;
        /* clear separate outline texture (not used when using combined) */
        start_outline_texture = NULL;
    }

    /*SETTINGS*/
    /* SETTINGS with outline (tighter letter spacing) */
    {
        const char* txt = "Settings";
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2;
        int outline_px = 2;
        SDL_Rect out_r = {0};
        settings_texture = CreateOutlinedTextTexture(txt, font, fg_col, outline_col, outline_px, gap, &out_r);
        settings_rect.w = out_r.w; settings_rect.h = out_r.h; settings_rect.x = (640 - out_r.w) / 2; settings_rect.y = 280;
        settings_outline_texture = NULL;
    }

    /*SKINS TEXT*/
    /* SKINS with outline (tighter letter spacing) */
    {
        const char* txt = "Skins";
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2;
        int outline_px = 2;
        SDL_Rect out_r = {0};
        skins_texture = CreateOutlinedTextTexture(txt, font, fg_col, outline_col, outline_px, gap, &out_r);
        skins_rect.w = out_r.w; skins_rect.h = out_r.h; skins_rect.x = (640 - out_r.w) / 2; skins_rect.y = 330;
        skins_outline_texture = NULL;
    }

    /*CHECK FOR UPDATES TEXT*/
    /* CHECK FOR UPDATES with outline (tighter letter spacing) */
    {
        const char* txt = "Check for updates";
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2;
        int outline_px = 2;
        SDL_Rect out_r = {0};
        Check_for_updates_texture = CreateOutlinedTextTexture(txt, font, fg_col, outline_col, outline_px, gap, &out_r);
        Check_for_updates_rect.w = out_r.w; Check_for_updates_rect.h = out_r.h; Check_for_updates_rect.x = (640 - out_r.w) / 2; Check_for_updates_rect.y = 230;
        Check_for_updates_outline_texture = NULL;
    }

    /*QUIT TEXT*/
    /* QUIT with outline (tighter letter spacing) */
    {
        const char* txt = "Quit";
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2;
        int outline_px = 2;
        SDL_Rect out_r = {0};
        quit_texture = CreateOutlinedTextTexture(txt, font, fg_col, outline_col, outline_px, gap, &out_r);
        quit_rect.w = out_r.w; quit_rect.h = out_r.h; quit_rect.x = (640 - out_r.w) / 2; quit_rect.y = 380;
        quit_outline_texture = NULL;
    }

    char TEXT_VERSION[64];
    snprintf(TEXT_VERSION, sizeof(TEXT_VERSION), "Version %d.%d.%d Beta 4", GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_PATCH);

    /*VERSION*/
    /* VERSION with outline (tighter letter spacing) */
    {
        SDL_Color outline_col = {0,0,0,255};
        SDL_Color fg_col = {255,255,255,255};
        int gap = 2;
        int outline_px = 2;
        SDL_Rect out_r = {0};
        version_texture = CreateOutlinedTextTexture(TEXT_VERSION, font, fg_col, outline_col, outline_px, gap, &out_r);
        version_rect.w = out_r.w; version_rect.h = out_r.h; version_rect.x = 10; version_rect.y = 10;
        version_outline_rect.w = out_r.w; version_outline_rect.h = out_r.h; version_outline_rect.x = 10 - (out_r.w - out_r.w)/2; version_outline_rect.y = 10 - (out_r.h - out_r.h)/2;
    }

    SDL_ShowCursor(SDL_ENABLE);

    /* Try to load optional menu background image */
    SDL_Surface* menu_bg_surface = IMG_Load("sprites/menu_bg.png");
    if(menu_bg_surface) {
        menu_bg_texture = SDL_CreateTextureFromSurface(renderer, menu_bg_surface);
        SDL_FreeSurface(menu_bg_surface);
    } else {
        SDL_Log("Menu background image not found: sprites/menu_bg.png, using solid color background");
    }

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
    save_settings_rect.y = minus_rect.y + minus_rect.h + 60;
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
    settings_saved_rect.y = save_settings_rect.y + save_settings_rect.h + 80;
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

    /*FULLSCREEN TEXT*/
    SDL_Surface* s_off = TTF_RenderText_Solid(font, "Fullscreen: Disabled", (SDL_Color){255, 255, 255, 255});
    tex_fullscreen_off = SDL_CreateTextureFromSurface(renderer, s_off);
    SDL_FreeSurface(s_off);

    SDL_Surface* s_on = TTF_RenderText_Solid(font, "Fullscreen: Enabled", (SDL_Color){255, 255, 255, 255});
    tex_fullscreen_on = SDL_CreateTextureFromSurface(renderer, s_on);
    SDL_FreeSurface(s_on);

    int fullscreenW = 0, fullscreenH = 0;
    SDL_QueryTexture(tex_fullscreen_off, NULL, NULL, &fullscreenW, &fullscreenH);
    fullscreen_rect.x = (640 - fullscreenW) / 2;
    fullscreen_rect.y = save_settings_rect.y - 40;
    fullscreen_rect.w = fullscreenW;
    fullscreen_rect.h = fullscreenH;

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
                exit(0);
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
                    if(volumen_text_game){SDL_DestroyTexture(volumen_text_game_texture); volumen_text_game_texture = NULL;}
                    if(plus_texture){ SDL_DestroyTexture(plus_texture); plus_texture = NULL; }
                    if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }
                    if(tex_fullscreen_off){ SDL_DestroyTexture(tex_fullscreen_off); tex_fullscreen_off = NULL;}
                    if(tex_fullscreen_on){ SDL_DestroyTexture(tex_fullscreen_on); tex_fullscreen_on = NULL;}
                    if(settings_saved_texture){ SDL_DestroyTexture(settings_saved_texture); settings_saved_texture = NULL; }

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

                /* Toggle fullscreen when clicking the fullscreen text */
                if(mx >= fullscreen_rect.x && mx <= fullscreen_rect.x + fullscreen_rect.w &&
                   my >= fullscreen_rect.y && my <= fullscreen_rect.y + fullscreen_rect.h)
                {
                    if(game_settings.fullscreen){
                        game_settings.fullscreen = false;
                        SDL_SetWindowFullscreen(window, 0);
                    }else {
                        game_settings.fullscreen = true;
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                        
                }

                if(mx >= save_settings_rect.x && mx <= save_settings_rect.x + save_settings_rect.w &&
                   my >= save_settings_rect.y && my <= save_settings_rect.y + save_settings_rect.h)
                {
                    bool settings_saved_show = true;
                    SaveSettings(&game_settings);
                    if(settings_saved_show){
                        SDL_RenderCopy(renderer, settings_saved_texture, NULL, &settings_saved_rect);
                    }
                    
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000); //Show message for 1 seconds
                    settings_saved_show = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, volumen_text_menu_texture, NULL, &volumen_text_menu_rect); /*VOLUMEN TEXT MENU*/
        SDL_RenderCopy(renderer, plus_texture, NULL, &plus_rect); /*+*/
        SDL_RenderCopy(renderer, minus_texture, NULL, &minus_rect); /*-*/
        SDL_RenderCopy(renderer, volumen_text_game_texture, NULL, &volumen_text_game_rect); /*VOLUMEN GAME TEXT*/

        if(game_settings.fullscreen){
            SDL_RenderCopy(renderer, tex_fullscreen_on, NULL, &fullscreen_rect);
        }else {
            SDL_RenderCopy(renderer, tex_fullscreen_off, NULL, &fullscreen_rect);
        }

        SDL_RenderCopy(renderer, plus_game_texture, NULL, &plus_game_rect); /*+ VOLUMEN GAME*/
        SDL_RenderCopy(renderer, minus_game_texture, NULL, &minus_game_rect); /*- VOLUMEN GAMEGAME*/
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
    huehuehue_rect.x = 250;
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
    goku_rect.x = huehuehue_rect.x + 150;
    goku_rect.y = 150 - huehuehue_rect.h;
    goku_rect.w = gokuW;
    goku_rect.h = gokuH;

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
    arrow2_rect.x = default_skin_rect.x + 50;
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
                if(default_skin_texture){ SDL_DestroyTexture(default_skin_texture); default_skin_texture = NULL; }
                if(goku_texture){ SDL_DestroyTexture(goku_texture); goku_texture = NULL; }
                if(arrow_texture){ SDL_DestroyTexture(arrow_texture); arrow_texture = NULL; }

                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                exit(0);
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
                        arrow2_rect.x += 150;
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
                        arrow2_rect.x -= 150;
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
                    if(default_skin_texture){ SDL_DestroyTexture(default_skin_texture); default_skin_texture = NULL; }
                    if(goku_texture){ SDL_DestroyTexture(goku_texture); goku_texture = NULL; }

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
        #if defined(__i386__) && !defined(__LP64__)
            args->url = "https://the-mutants-updates.web.app/update-x32";
        #else
            args->url = "https://the-mutants-updates.web.app/update";
        #endif
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

void Cleanup_Menu_Textures() {
    if(start_texture) { SDL_DestroyTexture(start_texture); start_texture = NULL; }
    if(quit_texture) { SDL_DestroyTexture(quit_texture); quit_texture = NULL; }
    if(version_texture) { SDL_DestroyTexture(version_texture); version_texture = NULL; }
    if(settings_texture) { SDL_DestroyTexture(settings_texture); settings_texture = NULL; }
    if(skins_texture) { SDL_DestroyTexture(skins_texture); skins_texture = NULL; }
    if(Check_for_updates_texture) { SDL_DestroyTexture(Check_for_updates_texture); Check_for_updates_texture = NULL; }
    if(menu_bg_texture) { SDL_DestroyTexture(menu_bg_texture); menu_bg_texture = NULL; }
    if(start_outline_texture) { SDL_DestroyTexture(start_outline_texture); start_outline_texture = NULL; }
    if(settings_outline_texture) { SDL_DestroyTexture(settings_outline_texture); settings_outline_texture = NULL; }
    if(skins_outline_texture) { SDL_DestroyTexture(skins_outline_texture); skins_outline_texture = NULL; }
    if(Check_for_updates_outline_texture) { SDL_DestroyTexture(Check_for_updates_outline_texture); Check_for_updates_outline_texture = NULL; }
    if(quit_outline_texture) { SDL_DestroyTexture(quit_outline_texture); quit_outline_texture = NULL; }
    if(version_outline_texture) { SDL_DestroyTexture(version_outline_texture); version_outline_texture = NULL; }
}

int Update_State_Menu()
{
    bool menu_running = true;
    while(menu_running) //Main loop
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                Cleanup_Menu_Textures();

                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                exit(0);
                return 0;
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= start_rect.x && mx <= start_rect.x + start_rect.w &&
                   my >= start_rect.y && my <= start_rect.y + start_rect.h)
                {
                    Cleanup_Menu_Textures();
                    
                    TTF_CloseFont(font);
                    state_game_ready = false;
                    state_menu_ready = false;
                    game_state = STATE_GAME;
                    
                    menu_running = false;
                    return 0;
                }else if(mx >= quit_rect.x && mx <= quit_rect.x + quit_rect.w &&
                   my >= quit_rect.y && my <= quit_rect.y + quit_rect.h)
                {
                    Cleanup_Menu_Textures();

                    TTF_CloseFont(font);
                    TTF_Quit();
                    game_state = STATE_EXIT;
                    menu_running = false;
                    return 0;
                }else if(mx >= settings_rect.x && mx <= settings_rect.x + settings_rect.w &&
                   my >= settings_rect.y && my <= settings_rect.y + settings_rect.h)
                {
                    Cleanup_Menu_Textures();

                    State_Config();
                    /* Recreate menu textures after returning from config */
                    Init_State_Menu();
                }else if(mx >= skins_rect.x && mx <= skins_rect.x + skins_rect.w &&
                   my >= skins_rect.y && my <= skins_rect.y + skins_rect.h)
                {
                    Cleanup_Menu_Textures();

                    State_Skins();
                    /* Recreate menu textures after returning from skins */
                    Init_State_Menu();
                }else if(mx >= Check_for_updates_rect.x && mx <= Check_for_updates_rect.x + Check_for_updates_rect.w &&
                my >= Check_for_updates_rect.y && my <= Check_for_updates_rect.y + Check_for_updates_rect.h)
                {
                    Cleanup_Menu_Textures();

                    StateUpdate();
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(menu_bg_texture) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(menu_bg_texture, NULL, NULL, &texW, &texH);
            if(texW > 0 && texH > 0) {
                const float winW = 640.0f;
                const float winH = 480.0f;
                float scale = fminf(winW / (float)texW, winH / (float)texH);
                int dstW = (int)(texW * scale);
                int dstH = (int)(texH * scale);
                SDL_Rect dst = { (int)((winW - dstW) / 2.0f), (int)((winH - dstH) / 2.0f), dstW, dstH };
                SDL_RenderCopy(renderer, menu_bg_texture, NULL, &dst);
            }
        } else {
            /* Fallback gradient similar to the attached background image */
            DrawVerticalGradient(renderer, (SDL_Color){167,240,226,255}, (SDL_Color){83,190,164,255});
        }

        /* Render outlines first (if any), then main text so outlines appear as borders */
        if(start_outline_texture) SDL_RenderCopy(renderer, start_outline_texture, NULL, &start_outline_rect);
        if(settings_outline_texture) SDL_RenderCopy(renderer, settings_outline_texture, NULL, &settings_outline_rect);
        if(skins_outline_texture) SDL_RenderCopy(renderer, skins_outline_texture, NULL, &skins_outline_rect);
        if(Check_for_updates_outline_texture) SDL_RenderCopy(renderer, Check_for_updates_outline_texture, NULL, &Check_for_updates_outline_rect);
        if(quit_outline_texture) SDL_RenderCopy(renderer, quit_outline_texture, NULL, &quit_outline_rect);
        if(version_outline_texture) SDL_RenderCopy(renderer, version_outline_texture, NULL, &version_outline_rect);

        SDL_RenderCopy(renderer, start_texture, NULL, &start_rect); //START TEXT
        SDL_RenderCopy(renderer, quit_texture, NULL, &quit_rect); //QUIT TEXT
        SDL_RenderCopy(renderer, Check_for_updates_texture, NULL, &Check_for_updates_rect); //CHECK FOR UPDATES TEXT
        SDL_RenderCopy(renderer, skins_texture, NULL, &skins_rect); //SKINS TEXT
        SDL_RenderCopy(renderer, version_texture, NULL, &version_rect); //VERSION TEXT
        SDL_RenderCopy(renderer, settings_texture, NULL, &settings_rect); //SETTINGS TEXT

        SDL_RenderPresent(renderer);
    }

    return 0;
}

/* Create a texture by rendering each glyph individually allowing custom letter spacing.
 * gap_adjust: pixels to subtract between glyphs (use positive to tighten spacing)
 */
static SDL_Texture* CreateTextTextureWithGlyphs(const char* text, TTF_Font* font, SDL_Color color, int gap_adjust, SDL_Rect* out_rect)
{
    int len = (int)strlen(text);
    if(len == 0) return NULL;

    SDL_Surface** glyphs = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * len);
    if(!glyphs) return NULL;

    int totalW = 0;
    int maxH = 0;
    for(int i = 0; i < len; ++i) {
        Uint16 ch = (unsigned char)text[i];
        glyphs[i] = TTF_RenderGlyph_Blended(font, ch, color);
        if(!glyphs[i]) {
            // cleanup
            for(int j = 0; j < i; ++j) SDL_FreeSurface(glyphs[j]);
            free(glyphs);
            return NULL;
        }
        totalW += glyphs[i]->w;
        if(i > 0) totalW -= gap_adjust;
        if(glyphs[i]->h > maxH) maxH = glyphs[i]->h;
    }

    SDL_Surface* master = SDL_CreateRGBSurfaceWithFormat(0, totalW, maxH, 32, SDL_PIXELFORMAT_RGBA32);
    if(!master) {
        for(int i = 0; i < len; ++i) SDL_FreeSurface(glyphs[i]);
        free(glyphs);
        return NULL;
    }
    SDL_SetSurfaceBlendMode(master, SDL_BLENDMODE_BLEND);
    SDL_FillRect(master, NULL, SDL_MapRGBA(master->format, 0, 0, 0, 0));

    int x = 0;
    for(int i = 0; i < len; ++i) {
        SDL_Rect dst = { x, (maxH - glyphs[i]->h) / 2, glyphs[i]->w, glyphs[i]->h };
        SDL_BlitSurface(glyphs[i], NULL, master, &dst);
        x += glyphs[i]->w - gap_adjust;
        SDL_FreeSurface(glyphs[i]);
    }

    free(glyphs);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, master);
    if(out_rect) {
        out_rect->x = 0; out_rect->y = 0; out_rect->w = master->w; out_rect->h = master->h;
    }
    SDL_FreeSurface(master);
    return tex;
}

static SDL_Texture* CreateOutlinedTextTexture(const char* text, TTF_Font* font, SDL_Color fg_color, SDL_Color outline_color, int outline_px, int gap_adjust, SDL_Rect* out_rect)
{
    int len = (int)strlen(text);
    if(len == 0) return NULL;

    SDL_Surface** outline_glyphs = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * len);
    SDL_Surface** main_glyphs = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * len);
    if(!outline_glyphs || !main_glyphs) { free(outline_glyphs); free(main_glyphs); return NULL; }

    int totalW = 0;
    int maxH = 0;
    for(int i = 0; i < len; ++i) {
        Uint16 ch = (unsigned char)text[i];
        outline_glyphs[i] = TTF_RenderGlyph_Blended(font, ch, outline_color);
        main_glyphs[i] = TTF_RenderGlyph_Blended(font, ch, fg_color);
        if(!main_glyphs[i] || !outline_glyphs[i]) {
            for(int j = 0; j <= i; ++j) { if(outline_glyphs[j]) SDL_FreeSurface(outline_glyphs[j]); if(main_glyphs[j]) SDL_FreeSurface(main_glyphs[j]); }
            free(outline_glyphs); free(main_glyphs);
            return NULL;
        }
        totalW += main_glyphs[i]->w;
        if(i > 0) totalW -= gap_adjust;
        if(main_glyphs[i]->h > maxH) maxH = main_glyphs[i]->h;
    }

    int outW = totalW + 2*outline_px;
    int outH = maxH + 2*outline_px;

    SDL_Surface* master = SDL_CreateRGBSurfaceWithFormat(0, outW, outH, 32, SDL_PIXELFORMAT_RGBA32);
    if(!master) {
        for(int i = 0; i < len; ++i) { SDL_FreeSurface(outline_glyphs[i]); SDL_FreeSurface(main_glyphs[i]); }
        free(outline_glyphs); free(main_glyphs);
        return NULL;
    }
    SDL_SetSurfaceBlendMode(master, SDL_BLENDMODE_BLEND);
    SDL_FillRect(master, NULL, SDL_MapRGBA(master->format, 0, 0, 0, 0));

    // Blit outline glyphs with offsets to create border
    int x = 0;
    for(int i = 0; i < len; ++i) {
        int glyphW = outline_glyphs[i]->w;
        int glyphH = outline_glyphs[i]->h;
        for(int oy = -outline_px; oy <= outline_px; ++oy) {
            for(int ox = -outline_px; ox <= outline_px; ++ox) {
                SDL_Rect dst = { x + outline_px + ox, outline_px + (maxH - glyphH) / 2 + oy, glyphW, glyphH };
                SDL_BlitSurface(outline_glyphs[i], NULL, master, &dst);
            }
        }
        x += glyphW - gap_adjust;
    }

    // Blit main glyphs centered over outline
    x = 0;
    for(int i = 0; i < len; ++i) {
        int glyphW = main_glyphs[i]->w;
        int glyphH = main_glyphs[i]->h;
        SDL_Rect dst = { x + outline_px, outline_px + (maxH - glyphH) / 2, glyphW, glyphH };
        SDL_BlitSurface(main_glyphs[i], NULL, master, &dst);
        x += glyphW - gap_adjust;
    }

    for(int i = 0; i < len; ++i) { SDL_FreeSurface(outline_glyphs[i]); SDL_FreeSurface(main_glyphs[i]); }
    free(outline_glyphs); free(main_glyphs);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, master);
    if(out_rect) { out_rect->x = 0; out_rect->y = 0; out_rect->w = master->w; out_rect->h = master->h; }
    SDL_FreeSurface(master);
    return tex;
}