/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: zombie_waves.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "zombie_waves.h"
#include "delta_time.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
extern int num_zombies;
extern float deltaTime;

int InitWave(WAVE *w, int n_zombies, float min_cooldown, float max_cooldown, float delay_time_start){

    w->num_zombies = n_zombies;
    w->zombies_spawned = 0;
    w->spawn_active = true;
    w->spawn_timer = -delay_time_start;
    w->min_cooldown = min_cooldown;
    w->max_cooldown = max_cooldown;
    w->delay_time_start = delay_time_start;
    w->timer = 0.0f;
    return 1;
}

void DeinitWave(WAVE *w){
    memset(w, 0, sizeof(WAVE));
}

int UpdateWave(WAVE* w, ZOMBIE *z){

    w->timer += deltaTime;
    SDL_Log("UpdateWave: timer=%.2f, delay_start=%.2f, spawn_timer=%.2f, spawn_active=%d, zombies_spawned=%d/%d", w->timer, w->delay_time_start, w->spawn_timer, w->spawn_active, w->zombies_spawned, w->num_zombies);

    if(w->timer >= w->delay_time_start)
    {
        if(w->min_cooldown == 0.0f && w->max_cooldown == 0.0f && w->delay_time_start == 0.0f && w->spawn_active)
        {
            SDL_Log("LEVEL 4 instant wave: spawning all remaining zombies (%d)", w->num_zombies - w->zombies_spawned);
            while(w->zombies_spawned < w->num_zombies)
            {
                SpawnZombieRandom();
                w->zombies_spawned++;
            }
            w->spawn_active = false;
            return -1;
        }
        else
        {
            w->spawn_timer += deltaTime;
            SDL_Log("UpdateWave inner: spawn_timer=%.2f", w->spawn_timer);

            if(w->spawn_timer >= 0 && w->zombies_spawned < w->num_zombies && w->spawn_active)
            {
                SDL_Log("SPAWNING ZOMBIE NOW");
                SpawnZombieRandom();
                w->zombies_spawned++;
                float random_cooldown = ((float)rand() / RAND_MAX) * (w->max_cooldown - w->min_cooldown) + w->min_cooldown;
                w->spawn_timer = -random_cooldown;
                SDL_Log("Spawned zombie, next cooldown: %.2f, total zombies: %d", random_cooldown, w->zombies_spawned);
            }

            if(w->zombies_spawned >= w->num_zombies){
                w->spawn_active = false;
            }
        }

        if(!w->spawn_active && num_zombies == 0)
        {
            printf("num_zombies:%d\n", num_zombies);
            fflush(stdout);
            SDL_Log("num_zombies:%d", num_zombies);
            SDL_Log("WAVE FINISHED");
            w->spawn_active = true;
            w->zombies_spawned = 0;
            return -1;
        }
    }


    return 1;

}