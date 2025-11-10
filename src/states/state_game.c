/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Sebas-1423)
File: state_menu.c

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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* backgroundTexture = NULL; //Background texture
SDL_Rect backgroundRect;
extern StatesPlayer states_player;
StatesPlayer last_states_player = IDLE;
SDL_RendererFlip player_flip = SDL_FLIP_NONE;

extern bool is_jumping;
extern float velocity_y;
extern float jump_force;

extern int current_frame;
extern Uint32 last_update_time;
extern float deltaTime;
extern int frame_duration;

/*VARIABLES STAMINA*/
float stamina = 25.0f;

const int ANCHO_MAXIMO_BARRA = 300;
const int ALTURA_BARRA = 20;

const int POS_X_BARRA = (640 / 2) - (ANCHO_MAXIMO_BARRA / 2);
const int POS_Y_BARRA = (480 - ALTURA_BARRA - 40);

void CheckChangeStatePlayer()
{
    if(states_player != last_states_player){
        current_frame = 0;
        last_update_time = SDL_GetTicks();
    }

    last_states_player = states_player;
}

int Init_State_Game()
{
    /*Load BACKGROUND test main menu test*/
    SDL_Surface* background_surface = IMG_Load("sprites/CHIP8-PS2-BACKGROUND.png");

    if(background_surface == NULL)
        printf("Unable to load image %s! SDL_image Error: %s\n", "sprites/CHIP8-PS2-BACKGROUND.png", IMG_GetError());

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface); //Convert surface to texture
    SDL_FreeSurface(background_surface); //Free loaded surface

    //Background texture properties
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = 640;
    backgroundRect.h = 480;

    LoadSpritesPlayer(); //Load player sprites
    LoadSpritesZombies(); //Load zombies sprites
    /* init a test zombie (x,y) */
    InitZombie(150.0f, 250.0f);

    return 0;
}

void UpdateAnimsPLAYER()
{
    CheckChangeStatePlayer();

    int max_frames_for_state;
    
    if (states_player == IDLE) {
        max_frames_for_state = IDLE_FRAMES;
    }else if (states_player == WALK) {
        max_frames_for_state = WALK_FRAMES;
    }else if (states_player == JUMP) {
        max_frames_for_state = JUMP_FRAMES;
    }else {
        return; 
    }

    UpdateANIM(max_frames_for_state);
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
        bool is_moving = state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT];

        if(state[SDL_SCANCODE_ESCAPE]){
            SDL_Quit();
            return 0;
        }

        /* si shift esta pulsado, el jugador obtiene velocidad*/
        if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && is_moving){
            SetPlayerSpeedMultiplier(1.8f); /* boost: 80% faster (adjustable) */
            stamina -= 6.0f * deltaTime;
            frame_duration = FRAME_DURATION / 2; // Aumenta la velocidad de la animación

            if(stamina <= 0.0f){
            ResetPlayerSpeed();
            stamina = 0.0f;
            frame_duration = FRAME_DURATION;
            }

        } else {
            ResetPlayerSpeed();
            frame_duration = FRAME_DURATION;
        }

        if(stamina > 25.0f){
            stamina = 25.0f;
        }
        
        //SDL_Log("Stamina: %.2f\n", stamina); // DEBUG STAMINA
        stamina += 3.0f * deltaTime;

        if(state[SDL_SCANCODE_LEFT]){
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

        if(state[SDL_SCANCODE_RIGHT]){
            input_state = WALK;
            player_flip = SDL_FLIP_NONE;
            PlayerForward();
        }

        // Decide el estado final del jugador
        if (is_jumping) {
            states_player = JUMP; // Mantiene JUMP si está saltando
        } else {
            states_player = input_state; // Usa el estado de entrada
        }

    UpdateAnimsPLAYER();
    UpdateDeltaTime();
    UpdateJump();

    /* update zombies AI and movement */
    UpdateZombies();

        /*TESTING*/
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //clean screen with black color
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect); /*TEXTURE TEST :)*/

        RenderBarStamina(); //Render stamina bar

    RenderPlayer(player_flip); //Render player
    RenderZombies();

        SDL_RenderPresent(renderer);    
    }

    return 0;
}
