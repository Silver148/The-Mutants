#include "zombie_waves.h"
#include "delta_time.h"
#include <SDL2/SDL.h>
extern int num_zombies;
extern float deltaTime;

int InitWave(WAVE *w, int n_zombies, float spawn_cooldown){

    w->num_zombies = n_zombies;
    w->spawn_timer = 0.0f;
    w->spawn_cooldown = spawn_cooldown;

    return 1;
}

int UpdateWave(WAVE* w, ZOMBIE *z){

    w->spawn_timer += deltaTime;
    //SDL_Log("spawn_timer %f\n", w->spawn_timer);

    if(w->spawn_timer >= w->spawn_cooldown && num_zombies < w->num_zombies)
    {
        SpawnZombieRandom();
        w->spawn_timer = 0.0f;
        SDL_Log("n_zombies:%d\n", num_zombies);
    }

    if(z->id >= w->num_zombies){
        SDL_Log("WAVE FINISHED\n");
        return -1;
    }

    return 1;

}