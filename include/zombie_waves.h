#ifndef ZOMBIE_WAVES_H
#define ZOMBIE_WAVES_H
#include "zombies.h"

typedef struct{

    int num_zombies;
    int zombies_speed;
    float spawn_timer;
    float spawn_cooldown;
    float delay_time_start;
    float timer;
} WAVE;

int InitWave(WAVE *w, int n_zombies, float spawn_cooldown, float delay_time_start);
void DeinitWave(WAVE *w);
int UpdateWave(WAVE* w, ZOMBIE *z);

#endif