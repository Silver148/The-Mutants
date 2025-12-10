#ifndef ZOMBIE_WAVES_H
#define ZOMBIE_WAVES_H
#include "zombies.h"

typedef struct{

    int num_zombies;
    int zombies_speed;
    float spawn_timer;
    float spawn_cooldown;
} WAVE;

int InitWave(WAVE *w, int n_zombies, float spawn_cooldown);
int UpdateWave(WAVE* w, ZOMBIE *z);

#endif