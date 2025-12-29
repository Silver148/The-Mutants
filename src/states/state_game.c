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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* backgroundTexture = NULL; //Background texture
SDL_Rect backgroundRect;
SDL_Rect backgroundSrcRect = {0,0,0,0};
static int backgroundImgW = 0;
static int backgroundImgH = 0;
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
    /*Load BACKGROUND test main menu test*/
    SDL_Surface* background_surface = IMG_Load("sprites/cityminimap.png");

    if(background_surface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/cityminimap.png", IMG_GetError());

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface); //Convert surface to texture
    SDL_FreeSurface(background_surface); //Free loaded surface

    // Center-crop the background to fill window without vertical stretching
    int imgW = 0, imgH = 0;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &imgW, &imgH);
    backgroundImgW = imgW;
    backgroundImgH = imgH;
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

    return 0;
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

int Update_State_Game()
{
    while(1) //Main loop(TESTING)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_Quit();
                return 0;
            }
        }

        StatesPlayer input_state = IDLE; // Inicializa input_state

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        /*BOOL MOVEMENT*/
        bool is_moving = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D];

        if(state[SDL_SCANCODE_ESCAPE]){
            SDL_Quit();
            return 0;
        }

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
            /* spawn projectile from player's front */
            float px = GetPositionPlayerX() + (player_flip == SDL_FLIP_NONE ? PLAYER_WIDTH : -8);
            float py = GetPositionPlayerY() + (PLAYER_HEIGHT / 2);
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
                StartPlayerShootingWalk();
            } else {
                StartPlayerShooting();
            }
        } else {
            StopPlayerShooting();
        }

        // Decide el estado final del jugador (no sobrescribir SHOOT mientras se reproduce)
        if (is_jumping) {
            states_player = JUMP; // Mantiene JUMP si está saltando
        } else {
            if (!IsPlayerShooting()) {
                states_player = input_state; // Usa el estado de entrada
            }
            // if shooting, keep SHOOT until animation finishes
        }

    UpdateAnimsPLAYER();
    UpdateDeltaTime();
    UpdateJump();
    CheckIfPlayerIsDead();

    /* update zombies AI and movement */
    UpdateZombies();
    
    UpdateProjectiles();

        /*TESTING*/
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //clean screen with black color
        SDL_RenderClear(renderer);

        if(backgroundSrcRect.w != 0 && backgroundSrcRect.h != 0) {
            // adjust backgroundSrcRect to follow player when near edges
            {
                float px = GetPositionPlayerX();
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
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect); /*TEXTURE TEST :)*/
        }

        RenderBarHealth(); // Render player health
        RenderBarStamina(); //Render stamina bar

        RenderPlayer(player_flip); //Render player

        RenderZombies(); //Render zombies
        RenderProjectiles(); // bullets

        #ifdef DEBUG
        ShowHitboxPlayer();
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies[i].alive) {
                ShowHitboxZombie(i);
            }
        }
        #endif

        SDL_RenderPresent(renderer);    
    }

    return 0;
}