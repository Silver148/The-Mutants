#ifndef ZOMBIE_WAVES_H
#define ZOMBIE_WAVES_H
#include <stdbool.h>
#include "zombies.h"

typedef struct{

    int num_zombies;
    int zombies_speed;
    int zombies_spawned;
    bool spawn_active;
    float spawn_timer;
    float min_cooldown;
    float max_cooldown;
    float delay_time_start;
    float timer;
} WAVE;

int InitWave(WAVE *w, int n_zombies, float min_cooldown, float max_cooldown, float delay_time_start);
void DeinitWave(WAVE *w);
int UpdateWave(WAVE* w, ZOMBIE *z);

#endif