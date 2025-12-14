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
#include <SDL2/SDL.h>
extern int num_zombies;
extern float deltaTime;

int InitWave(WAVE *w, int n_zombies, float spawn_cooldown, float delay_time_start){

    w->num_zombies = n_zombies;
    w->spawn_timer = 0.0f;
    w->spawn_cooldown = spawn_cooldown;
    w->delay_time_start = delay_time_start;
    w->timer = 0.0f;

    return 1;
}

int UpdateWave(WAVE* w, ZOMBIE *z){

    static bool spawn = true;
    static int zombies_spawned = 0;
    w->timer += deltaTime;
    //SDL_Log("timer: %f\n", w->timer);

    if(w->timer >= w->delay_time_start)
    {
        w->spawn_timer += deltaTime;
        //SDL_Log("spawn_timer %f\n", w->spawn_timer);

        if(w->spawn_timer >= w->spawn_cooldown && zombies_spawned < w->num_zombies && spawn)
        {
            SpawnZombieRandom();
            zombies_spawned++;
            w->spawn_timer = 0.0f;
            SDL_Log("n_zombies:%d\n", num_zombies);
        }

        if(zombies_spawned >= w->num_zombies){
            spawn = false;
        }

        if(!spawn && num_zombies == 0)
        {
            SDL_Log("num_zombies:%d\n", num_zombies);
            SDL_Log("WAVE FINISHED\n");
            spawn = true;
            zombies_spawned = 0;
            return -1;
        }
    }


    return 1;

}