/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: state_game.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/*
STATE GAME :D by Juan Yaguaro And Abel Ferrer
*/

#include "states.h"
#include "global_vars.h"
#include "player.h"
#include "zombies.h"
#include "delta_time.h"
#include "anim_manager.h"
#include "music.h"
#include "projectiles.h"
#include "zombie_waves.h"
#include "weapons_system.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

SDL_Texture* backgroundTexture = NULL; //Background texture
SDL_Rect backgroundRect;
SDL_Rect backgroundSrcRect = {0,0,0,0};
int backgroundImgW = 0;
int backgroundImgH = 0;
/* world barrier (right-side) */
int worldBarrierX = 0;
int worldBarrierLeftX = 0;
extern StatesPlayer states_player;
StatesPlayer last_states_player = IDLE;
SDL_RendererFlip player_flip = SDL_FLIP_NONE;

extern bool is_jumping;
extern float velocity_y;
extern float jump_force;
extern int health;

extern IDLE_PLAYER idle_player;
extern WALK_PLAYER walk_player;
extern JUMP_PLAYER jump_player;

extern float deltaTime;

/*VARIABLES STAMINA*/
float stamina = 25.0f;

const int ANCHO_MAXIMO_BARRA = 300;
const int ALTURA_BARRA = 20;

const int POS_X_BARRA = (640 / 2) - (ANCHO_MAXIMO_BARRA / 2);
const int POS_Y_BARRA = (480 - ALTURA_BARRA - 40);

extern ZOMBIE zombies[MAX_ZOMBIES];/*zombies instance (from zombies.c) */

TTF_Font* font_kills = NULL;
SDL_Surface* tmp_surf_kills_text = NULL;
SDL_Texture* kills_text_texture = NULL;

char kills_text[32] = {0};
extern int counter_kills;
SDL_Rect kills_text_rect;

bool is_paused = false;
TTF_Font* font_paused = NULL;
SDL_Texture* paused_texture = NULL;

TTF_Font* press_esc = NULL;
SDL_Texture* press_esc_texture = NULL;
SDL_Texture* exit_to_menu = NULL;

SDL_Rect paused_rect;
SDL_Rect press_esc_rect;
SDL_Rect exit_to_menu_rect;

TTF_Font* font_ammunitions = NULL;
SDL_Texture* font_ammunitions_texture = NULL;
SDL_Rect font_ammunitions_rect;

extern STATES game_state;

extern bool state_game_ready;
extern bool state_menu_ready;

extern int Ammunition;
Ammunitions ammunitions[128];
SDL_Texture* bullets_tex = NULL;

extern int counter_kills;
static int last_ammo_spawn_kill = 0;

/* Update or recreate the kills text texture from the current kills count */
void UpdateKillsTexture(int kills)
{
    if (!font_kills) return;

    char buf[64];
    snprintf(buf, sizeof(buf), "Kills: %d", kills);

    SDL_Surface* surf = TTF_RenderText_Blended(font_kills, buf, (SDL_Color){255,255,255,255});
    if (!surf) {
        SDL_Log("TTF_RenderText_Blended failed: %s", TTF_GetError());
        return;
    }

    SDL_Texture* new_tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!new_tex) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return;
    }

    if (kills_text_texture) {
        SDL_DestroyTexture(kills_text_texture);
    }
    kills_text_texture = new_tex;
    SDL_SetTextureBlendMode(kills_text_texture, SDL_BLENDMODE_BLEND);

    int w = 0, h = 0;
    SDL_QueryTexture(kills_text_texture, NULL, NULL, &w, &h);
    kills_text_rect.x = 640 - w - 10;
    kills_text_rect.y = 10;
    kills_text_rect.w = w;
    kills_text_rect.h = h;
    SDL_Log("UpdateKillsTexture: created texture for %d (w=%d h=%d)", kills, w, h);
}

void UpdateAmmunitions(){
    char Ammunitions_text[64];
    if(!font_ammunitions) return;

    snprintf(Ammunitions_text, sizeof(Ammunitions_text), "Ammunitions: %d", Ammunition);

    SDL_Surface* surf_ammunitions = TTF_RenderText_Solid(font_ammunitions, Ammunitions_text, (SDL_Color){255,255,255,255});

    SDL_Texture* tex_ammunitions = SDL_CreateTextureFromSurface(renderer, surf_ammunitions);
    SDL_FreeSurface(surf_ammunitions);

    if(!tex_ammunitions) return;

    if(font_ammunitions_texture){
        SDL_DestroyTexture(font_ammunitions_texture);
    }
    font_ammunitions_texture = tex_ammunitions;
    SDL_SetTextureBlendMode(font_ammunitions_texture, SDL_BLENDMODE_BLEND);

    int w_am = 0, h_am = 0;
    SDL_QueryTexture(font_ammunitions_texture, NULL, NULL, &w_am, &h_am);
    font_ammunitions_rect.x = kills_text_rect.x - kills_text_rect.w - font_ammunitions_rect.w - 20;
    font_ammunitions_rect.y = 10;
    font_ammunitions_rect.w = w_am;
    font_ammunitions_rect.h = h_am;
    SDL_Log("UpdateAmmunitions: created texture for %d (w=%d h=%d)", Ammunition, w_am, h_am);
}

void CleanupKillsTexture()
{
    TTF_CloseFont(font_kills);
    font_kills = NULL;
    if (kills_text_texture) {
        SDL_DestroyTexture(kills_text_texture);
        kills_text_texture = NULL;
    }
}

void CleanupAmmunitions(){
    TTF_CloseFont(font_ammunitions);
    font_ammunitions = NULL;

    if(font_ammunitions_texture){
        SDL_DestroyTexture(font_ammunitions_texture);
        font_ammunitions_texture = NULL;
    }

    for(int i = 0; i < 128; i++){
        ammunitions[i].dest_bullet.x = -1;
        ammunitions[i].dest_bullet.y = -1;
        ammunitions[i].tex_bullet = NULL;
        ammunitions[i].dest_bullet.w = 0;
        ammunitions[i].dest_bullet.h = 0;
    }
}

void GamePaused()
{
    SDL_RenderCopy(renderer, paused_texture, NULL, &paused_rect);
    SDL_RenderCopy(renderer, press_esc_texture, NULL, &press_esc_rect);
    SDL_RenderCopy(renderer, exit_to_menu, NULL, &exit_to_menu_rect);
    SDL_RenderPresent(renderer);
}

void CleanGamePaused()
{
    TTF_CloseFont(font_paused);
    TTF_CloseFont(press_esc);
    press_esc = NULL;
    font_paused = NULL;

    if(paused_texture){
        SDL_DestroyTexture(paused_texture);
        paused_texture = NULL;
    }

    if(press_esc_texture){
        SDL_DestroyTexture(press_esc_texture);
        press_esc_texture = NULL;
    }

    if(exit_to_menu){
        SDL_DestroyTexture(exit_to_menu);
        exit_to_menu = NULL;
    }
}

void ShowHitboxPlayer()
{
    Hitbox player_hitbox = GetPlayerHitbox();

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for hitbox
    SDL_Rect hitboxRect = { (int)player_hitbox.x - backgroundSrcRect.x, (int)player_hitbox.y - backgroundSrcRect.y, player_hitbox.w, player_hitbox.h };
    SDL_RenderDrawRect(renderer, &hitboxRect);
}

void CheckChangeStatePlayer()
{
    if(states_player != last_states_player){
        switch(last_states_player) {
            case IDLE:
                Animation_Reset(&idle_player.idle_anim);
                break;
            case WALK:
                Animation_Reset(&walk_player.walk_anim);
                break;
            case JUMP:
                Animation_Reset(&jump_player.jump_anim);
                break;
        }
    }
    
    last_states_player = states_player;
}

int Init_State_Game()
{
    if(state_game_ready){ return -1;}

    font_kills = TTF_OpenFont("fonts/Half Awake.ttf", 24);
    if (!font_kills) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return -1;
    }

    font_ammunitions = TTF_OpenFont("fonts/Half Awake.ttf", 12);
    if(!font_ammunitions){
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return -1;
    }

    /* Create initial kills texture from current counter */
    UpdateKillsTexture(counter_kills);

    SDL_Log("Cargando fondo...");
    /*Load BACKGROUND test main menu test*/
    SDL_Surface* background_surface = IMG_Load("sprites/cityminimap.png");

    if(background_surface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/cityminimap.png", IMG_GetError());

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface); //Convert surface to texture
    SDL_FreeSurface(background_surface); //Free loaded surface

    int imgW = 0, imgH = 0;
    // Center-crop the background to fill window without vertical stretching
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &imgW, &imgH);
    backgroundImgW = imgW;
    backgroundImgH = imgH;
    /* place the world barrier at the far right edge of the background */
    worldBarrierX = backgroundImgW - WORLD_BARRIER_WIDTH;
    /* left barrier at the far left (inside the image) */
    worldBarrierLeftX = 0; /* could be WORLD_BARRIER_WIDTH to inset */
    const int winW = 640;
    const int winH = 480;
    if(imgW > 0 && imgH > 0) {
        float imgAspect = (float)imgW / (float)imgH;
        float winAspect = (float)winW / (float)winH;

        SDL_Rect srcRect = {0,0,imgW,imgH};
        // choose source rect to crop the image to the window aspect
        if(imgAspect > winAspect) {
            // image is wider -> crop left/right
            int srcW = (int)(imgH * winAspect);
            srcRect.x = (imgW - srcW) / 2;
            srcRect.w = srcW;
        } else if(imgAspect < winAspect) {
            // image is taller -> crop top/bottom
            int srcH = (int)(imgW / winAspect);
            srcRect.y = (imgH - srcH) / 2;
            srcRect.h = srcH;
        }
        // destination fills window
        backgroundRect.x = 0;
        backgroundRect.y = 0;
        backgroundRect.w = winW;
        backgroundRect.h = winH;

        // store the srcRect in the global backgroundSrcRect for use at render
        backgroundSrcRect = srcRect;
    } else {
        backgroundRect.x = 0;
        backgroundRect.y = 0;
        backgroundRect.w = winW;
        backgroundRect.h = winH;
    }

    LoadSpritesPlayer(); //Load player sprites
    LoadSpritesZombies(); //Load zombies sprites
    /* init Zombie system */
    InitZombieSys();
    InitProjectiles();

    font_paused = TTF_OpenFont("fonts/SYSTEMIA.ttf", 48);
    press_esc = TTF_OpenFont("fonts/SYSTEMIA.ttf", 24);

    if(!font_paused){
        SDL_Log("font_paused failed to load: %s", TTF_GetError());
        return -1;
    }

    SDL_Surface* tmp_surf_revolver_ammunition = IMG_Load("sprites/balas de revolver.png");
    bullets_tex = SDL_CreateTextureFromSurface(renderer, tmp_surf_revolver_ammunition);
    SDL_FreeSurface(tmp_surf_revolver_ammunition);

    for(int i = 0; i < 128; i++) {
        ammunitions[i].tex_bullet = bullets_tex;
    
        ammunitions[i].dest_bullet.x = -1; 
        ammunitions[i].dest_bullet.y = -1;
    
        int w, h;
        SDL_QueryTexture(bullets_tex, NULL, NULL, &w, &h);
        ammunitions[i].dest_bullet.w = w;
        ammunitions[i].dest_bullet.h = h;
    }

    /*PAUSED TEXTURE :)*/
    SDL_Surface* paused_surface = TTF_RenderText_Solid(font_paused, "GAME PAUSED :)", (SDL_Color){255, 0, 0, 255});
    SDL_Surface* press_esc_surface = TTF_RenderText_Solid(press_esc, "Press ESC to resume", (SDL_Color){255, 255, 255, 255});
    SDL_Surface* exit_to_menu_surface = TTF_RenderText_Solid(press_esc, "Exit to menu", (SDL_Color){255, 0, 0, 255});
    press_esc_texture = SDL_CreateTextureFromSurface(renderer, press_esc_surface);
    paused_texture = SDL_CreateTextureFromSurface(renderer, paused_surface);
    exit_to_menu = SDL_CreateTextureFromSurface(renderer, exit_to_menu_surface);
    SDL_FreeSurface(paused_surface);
    SDL_FreeSurface(press_esc_surface);
    SDL_FreeSurface(exit_to_menu_surface);

    int pW = 0, pH = 0;
    SDL_QueryTexture(paused_texture, NULL, NULL, &pW, &pH);
    paused_rect.x = (640 - pW) / 2;
    paused_rect.y = (480 - pH) / 2;
    paused_rect.w = pW;
    paused_rect.h = pH;

    int exitW = 0, exitH = 0;
    SDL_QueryTexture(exit_to_menu, NULL, NULL, &exitW, &exitH);
    exit_to_menu_rect.x = (640 - exitW) / 2;
    exit_to_menu_rect.y = paused_rect.y + pH + 20;
    exit_to_menu_rect.w = exitW;
    exit_to_menu_rect.h = exitH;

    int eW = 0, eH = 0;
    SDL_QueryTexture(press_esc_texture, NULL, NULL, &eW, &eH);
    press_esc_rect.x = (640 - eW) / 2;
    press_esc_rect.y = exit_to_menu_rect.y + exitH + 20;
    press_esc_rect.w = eW;
    press_esc_rect.h = eH;

    /*FONT AMMUNITIONS */
    UpdateAmmunitions();

    /* HIDE CURSOR */
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    return 0;
}

void CleanupBackground()
{
    if(backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }
}

/* Replace current background with image at `path` and recalc src/dest rects */
void SetBackgroundImage(const char* path)
{
    if (!path) return;

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }

    SDL_Surface* background_surface = IMG_Load(path);
    if (background_surface == NULL) {
        SDL_Log("SetBackgroundImage: Unable to load image %s! SDL_image Error: %s", path, IMG_GetError());
        return;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface);
    if (!backgroundTexture) {
        SDL_Log("SetBackgroundImage: Failed to create texture from %s", path);
        SDL_FreeSurface(background_surface);
        return;
    }

    int imgW = 0, imgH = 0;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &imgW, &imgH);
    backgroundImgW = imgW;
    backgroundImgH = imgH;
    worldBarrierX = backgroundImgW - WORLD_BARRIER_WIDTH;

    int winW = backgroundRect.w ? backgroundRect.w : 640;
    int winH = backgroundRect.h ? backgroundRect.h : 480;

    SDL_Rect srcRect = {0,0,imgW,imgH};
    if (imgW > 0 && imgH > 0) {
        float imgAspect = (float)imgW / (float)imgH;
        float winAspect = (float)winW / (float)winH;
        if (imgAspect > winAspect) {
            int srcW = (int)(imgH * winAspect);
            srcRect.x = (imgW - srcW) / 2;
            srcRect.w = srcW;
        } else if (imgAspect < winAspect) {
            int srcH = (int)(imgW / winAspect);
            srcRect.y = (imgH - srcH) / 2;
            srcRect.h = srcH;
        }
    }
    backgroundSrcRect = srcRect;

    SDL_FreeSurface(background_surface);
}

void UpdateAnimsPLAYER()
{
    CheckChangeStatePlayer();
    /* update player shoot animation if active */
    UpdatePlayerShootAnim();
}

void UpdateJump(){
    if(is_jumping){PlayerJump();}
}

void RenderBarStamina()
{   
    // 1. Calcular la proporción de la Stamina (entre 0 y 1)
    float proporcion = stamina / 25.0f;

    // 2. Calcular el nuevo ancho en píxeles
    int anchoActual = (int)(proporcion * ANCHO_MAXIMO_BARRA);

    SDL_Rect barraActual = {
    POS_X_BARRA,
    POS_Y_BARRA,
    anchoActual,
    ALTURA_BARRA
    };

    SDL_Rect marcoBarra = {POS_X_BARRA, POS_Y_BARRA, ANCHO_MAXIMO_BARRA, ALTURA_BARRA}; // Bar

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); //Bar background
    SDL_RenderFillRect(renderer, &marcoBarra);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //Stamina color
    SDL_RenderFillRect(renderer, &barraActual);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Border color
    SDL_RenderDrawRect(renderer, &marcoBarra);
}

void RenderBarHealth()
{
    /* Player health bar (top-left) */
    const int MAX_HEALTH = 170; /* CORREGÍ: debe ser 170 para coincidir con player.c */
    const int HEALTH_BAR_WIDTH = 200;
    const int HEALTH_BAR_HEIGHT = 16;
    const int HEALTH_POS_X = 20;
    const int HEALTH_POS_Y = 20;

    float proportion = (float)health / (float)MAX_HEALTH;
    if(proportion < 0.0f) proportion = 0.0f;
    if(proportion > 1.0f) proportion = 1.0f;

    int currentWidth = (int)(proportion * HEALTH_BAR_WIDTH);

    SDL_Rect healthRect = { HEALTH_POS_X, HEALTH_POS_Y, currentWidth, HEALTH_BAR_HEIGHT };
    SDL_Rect frameRect = { HEALTH_POS_X, HEALTH_POS_Y, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT };

    /* background */
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &frameRect);

    /* health fill (red) */
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthRect);

    /* border */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &frameRect);
}

void SpawnAmmunition() {

    int slot = -1;
    if (counter_kills > 0 && (counter_kills % 9) == 0 && counter_kills != last_ammo_spawn_kill) {
        for (int i = 0; i < 128; i++) {
            if (ammunitions[i].dest_bullet.x == -1) {
                slot = i;
                break;
            }
        }
        last_ammo_spawn_kill = counter_kills;
    }

    if (slot == -1) return;
    float player_x = GetPositionPlayerX();
    
    int spawn_x = 0; 
    do{
        spawn_x = rand() % (backgroundImgW - 100);

    }while(fabsf(spawn_x - player_x) < 250.0f);
    
    int spawn_y = 350;

    ammunitions[slot].dest_bullet.x = spawn_x;
    ammunitions[slot].dest_bullet.y = spawn_y;

    SDL_Log("Munición spawneada en el slot %d, pos: (%d, %d)", slot, spawn_x, spawn_y);
}

void UpdateAmmoCollection() {
    Hitbox player_hitbox = GetPlayerHitbox();
    SDL_Rect pRect = { (int)player_hitbox.x, (int)player_hitbox.y, player_hitbox.w, player_hitbox.h };
    
    for (int i = 0; i < 128; i++) {
        if (ammunitions[i].dest_bullet.x != -1) {
            
            if (SDL_HasIntersection(&pRect, &ammunitions[i].dest_bullet)) {
                /* Grant configured amount per ammo box */
                Ammunition += AMMO_BOX_AMOUNT;
                ammunitions[i].dest_bullet.x = -1;

                UpdateAmmunitions(); 
                SDL_Log("Municion recogida! +%d Total: %d", AMMO_BOX_AMOUNT, Ammunition);
            }
        }
    }
}

void RenderAmmunitionsItems() {
    for (int i = 0; i < 128; i++) {
        if (ammunitions[i].dest_bullet.x != -1) {
            SDL_Rect render_pos = ammunitions[i].dest_bullet;
            render_pos.x -= backgroundSrcRect.x;
            render_pos.y -= backgroundSrcRect.y;
            
            SDL_RenderCopy(renderer, bullets_tex, NULL, &render_pos);
        }
    }
}

int Update_State_Game()
{
    Uint32 start_fps = SDL_GetTicks();
    int frames = 0;
    while(!is_paused) //Main loop
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                CleanupBackground();
                CleanupZombieSystem();
                CleanupPlayer();
                CleanupProjectileSystem();
                CleanupKillsTexture();
                CleanupAmmunitions();
                TTF_Quit();
                SDL_Quit();
                exit(0);
                return 0;
            }

            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){;
                    is_paused = true;
                }
            }
        }

        StatesPlayer input_state = IDLE; // Inicializa input_state

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        /*BOOL MOVEMENT*/
        bool is_moving = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D];

        /*
        if(state[SDL_SCANCODE_ESCAPE]){
            SDL_Quit();
            return 0;
        }*/

        /* si shift esta pulsado, el jugador obtiene velocidad*/
        if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && is_moving){
            SetPlayerSpeedMultiplier(GetShiftMultiplierForSkin()); /* boost depends on skin */
            stamina -= 6.0f * deltaTime;
            
            // Modificar solo la velocidad de animación de walk cuando está caminando
            if(states_player == WALK) {
                walk_player.walk_anim.frame_duration = FRAME_DURATION_PLAYER / 2;
            }

            if(stamina <= 0.0f){
                ResetPlayerSpeed();
                stamina = 0.0f;
                // Restaurar velocidad normal de animación
                if(states_player == WALK) {
                    walk_player.walk_anim.frame_duration = FRAME_DURATION_PLAYER;
                }
            }

        } else {
            ResetPlayerSpeed();
            // Restaurar velocidad normal de animación
            if(states_player == WALK) {
                walk_player.walk_anim.frame_duration = FRAME_DURATION_PLAYER;
            }
        }

        if(stamina > 25.0f){
            stamina = 25.0f;
        }
        
        stamina += 3.0f * deltaTime;

        if(state[SDL_SCANCODE_A]){
            input_state = WALK;
            player_flip = SDL_FLIP_HORIZONTAL;
            PlayerBackward();
        }

        if(state[SDL_SCANCODE_SPACE]){
            if (!is_jumping) {
                input_state = JUMP; // Cambia a JUMP si no está saltando
                is_jumping = true;
                velocity_y = -jump_force; 
            }
        }

        if(state[SDL_SCANCODE_D]){
            input_state = WALK;
            player_flip = SDL_FLIP_NONE;
            PlayerForward();
        }

            /* Shooting: press Z to shoot (simple cooldown) */
            static float shoot_timer = 0.0f;
            if(shoot_timer > 0.0f) shoot_timer -= deltaTime;
        if(state[SDL_SCANCODE_Z] && shoot_timer <= 0.0f){
            /* spawn projectile from player's front
               Use the currently-displayed sprite frame height so the
               projectile originates from the visible sprite, preventing
               vertical desync when frame heights differ. */
            SDL_Rect* player_src = NULL;
            if (states_player == WALK) player_src = Animation_GetSourceRect(&walk_player.walk_anim);
            else if (states_player == JUMP) player_src = Animation_GetSourceRect(&jump_player.jump_anim);
            else player_src = Animation_GetSourceRect(&idle_player.idle_anim);
            int src_h = player_src ? player_src->h : PLAYER_HEIGHT;

            float px = GetPositionPlayerX() + (player_flip == SDL_FLIP_NONE ? PLAYER_WIDTH : -8);
            float py = GetPositionPlayerY() + (PLAYER_HEIGHT - src_h) + (src_h / 2.0f);
            float speed = 320.0f;
            float vx = (player_flip == SDL_FLIP_NONE) ? speed : -speed;
            {
                int base_damage = 25;
                int dmg = (int)(base_damage * GetProjectileDamageMultiplier());
                SpawnProjectile(px, py, vx, 0.0f, dmg); /* damage varies with skin */
            }
            /* spawn projectile, keep shoot animation started in continuous handler below */
            shoot_timer = 0.3f; /* 300ms between shots */
        }

        /* Start/stop continuous shooting animation based on Z key held
           Use walk-shoot when moving (A or D pressed) */
        if (state[SDL_SCANCODE_Z]) {
            if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D]) {
                if(CheckMunitions())
                    StartPlayerShootingWalk();
                else{
                    states_player = input_state;
                    StopPlayerShooting();
                }
                    
            } else {
                if(CheckMunitions())
                    StartPlayerShooting();
                else {
                    states_player = input_state;
                    StopPlayerShooting();
                }    
            }
        } else {
            StopPlayerShooting();
        }

        // Decide el estado final del jugador (no sobrescribir SHOOT mientras se reproduce)
        if (IsPlayerShooting()) {
        // Si está disparando, mantenemos el estado de disparo sin importar si salta o camina
            states_player = SHOOT; 
        } else if (is_jumping) {
            states_player = JUMP;
        } else {
        states_player = input_state; // IDLE o WALK
        }

        //If "is_paused" is true, enter paused loop
        while(is_paused){
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_ShowCursor(SDL_ENABLE);
            if(e.type == SDL_QUIT)
            {
                CleanupBackground();
                CleanupZombieSystem();
                CleanupPlayer();
                CleanupProjectileSystem();
                CleanupKillsTexture();
                CleanupAmmunitions();
                CleanGamePaused();
                TTF_Quit();
                SDL_Quit();
                exit(0);
                return 0;
            }

            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    UpdateDeltaTime();
                    SDL_ShowCursor(SDL_DISABLE);
                    is_paused = false;
                }
            }

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx >= exit_to_menu_rect.x && mx <= exit_to_menu_rect.x + exit_to_menu_rect.w && 
                my >= exit_to_menu_rect.y && my <= exit_to_menu_rect.y + exit_to_menu_rect.h)
                {
                    CleanupBackground();
                    CleanupZombieSystem();
                    CleanupPlayer();
                    CleanupProjectileSystem();
                    CleanupKillsTexture();
                    CleanupAmmunitions();
                    CleanGamePaused();

                    CloseMusic();
                    InitMusic();
                    PlayMusicStateMenu();

                    counter_kills = 0;
                    is_paused = false;

                    game_state = STATE_MENU;
                    state_game_ready = false;

                    /* CLEAR SCREN*/
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_RenderPresent(renderer);
                    return 0;
                }
            }
        }
        GamePaused();
        SDL_Delay(100); //CPU friendly loop when paused :)
    }

    //Update if not paused
    if(!is_paused) {
        UpdateAnimsPLAYER();
        UpdateDeltaTime();
        UpdateJump();
        if(CheckIfPlayerIsDead()){
            /* CLEAR SCREN*/
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            return 0;
        }
        UpdateZombies(); /* update zombies AI and movement */
        UpdateProjectiles();
        UpdateAmmoCollection();
        SpawnAmmunition();
    }
    
        /* Refresh kills texture when counter changes */
        static int last_counter_kills = -1;
        if (last_counter_kills != counter_kills) {
            UpdateKillsTexture(counter_kills);
            last_counter_kills = counter_kills;
        }

        /* REFRESH AMMUNITIONS TEXTURE :D*/
        static int last_ammunitions = -1;
        if(last_ammunitions != Ammunition){
            UpdateAmmunitions();
            last_ammunitions = Ammunition;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //clean screen with black color
        SDL_RenderClear(renderer);

        if(backgroundSrcRect.w != 0 && backgroundSrcRect.h != 0) {
            // adjust backgroundSrcRect to follow player when near edges
            {
                     /* use player's logical center for camera tracking to avoid
                         visual jumps when animation frame widths change */
                     float px = GetPositionPlayerX() + (PLAYER_WIDTH / 2.0f);
                     float py = GetPositionPlayerY();
                const int margin = 80; // px from edge before background moves
                int maxSrcX = (backgroundImgW > backgroundSrcRect.w) ? (backgroundImgW - backgroundSrcRect.w) : 0;
                int maxSrcY = (backgroundImgH > backgroundSrcRect.h) ? (backgroundImgH - backgroundSrcRect.h) : 0;

                if(px - backgroundSrcRect.x < margin) {
                    int newX = (int)px - margin;
                    if(newX < 0) newX = 0;
                    backgroundSrcRect.x = newX;
                } else if(backgroundSrcRect.x + backgroundSrcRect.w - px < margin) {
                    int newX = (int)px + margin - backgroundSrcRect.w;
                    if(newX > maxSrcX) newX = maxSrcX;
                    backgroundSrcRect.x = newX;
                }

                if(py - backgroundSrcRect.y < margin) {
                    int newY = (int)py - margin;
                    if(newY < 0) newY = 0;
                    backgroundSrcRect.y = newY;
                } else if(backgroundSrcRect.y + backgroundSrcRect.h - py < margin) {
                    int newY = (int)py + margin - backgroundSrcRect.h;
                    if(newY > maxSrcY) newY = maxSrcY;
                    backgroundSrcRect.y = newY;
                }
            }
                SDL_RenderCopy(renderer, backgroundTexture, &backgroundSrcRect, &backgroundRect);
        } else {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        }
        
        /* world barriers exist in logic but are invisible (no rendering) */

        RenderBarHealth(); // Render player health
        RenderBarStamina(); //Render stamina bar
        if (kills_text_texture) {
            SDL_RenderCopy(renderer, kills_text_texture, NULL, &kills_text_rect);
        }

        if(font_ammunitions_texture){
            SDL_RenderCopy(renderer, font_ammunitions_texture, NULL, &font_ammunitions_rect);
        }

        RenderPlayer(player_flip); //Render player

        RenderZombies(); //Render zombies
        RenderProjectiles(); // bullets
        RenderAmmunitionsItems(); // Render ammunition items on the ground

        #ifdef DEBUG
        ShowHitboxPlayer();
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies[i].alive) {
                ShowHitboxZombie(i);
            }
        }
        #endif

        SDL_RenderPresent(renderer);
        frames++;
        if (SDL_GetTicks() - start_fps >= 1000) {
            char title[50];
            sprintf(title, "The Mutant's - FPS: %d", frames);
            SDL_SetWindowTitle(window, title);
            frames = 0;
            start_fps = SDL_GetTicks();
        }  
    }

    return 0;
}