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
    w->timer += deltaTime;
    //SDL_Log("timer: %f\n", w->timer);

    if(w->timer >= w->delay_time_start)
    {
        w->spawn_timer += deltaTime;
        //SDL_Log("spawn_timer %f\n", w->spawn_timer);

        if(w->spawn_timer >= w->spawn_cooldown && num_zombies < w->num_zombies && spawn)
        {
            SpawnZombieRandom();
            w->spawn_timer = 0.0f;
            SDL_Log("n_zombies:%d\n", num_zombies);
        }

        if(z->id >= w->num_zombies){
            spawn = false;
        }

        if(!spawn && num_zombies == 0)
        {
            SDL_Log("num_zombies:%d\n", num_zombies);
            SDL_Log("WAVE FINISHED\n");
            spawn = true;
            return -1;
        }
    }


    return 1;

}