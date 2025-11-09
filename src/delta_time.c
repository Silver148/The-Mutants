/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light), Juan Yaguaro(aka silverhacker), Sebastián Colina(aka Sebas-1423)
File: delta_time.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/*

DELTA TIME MANAGEMENT by Juan Yaguaro

*/

#include "delta_time.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

float deltaTime = 0;
Uint32 lastTime = 0;
Uint32 currentTime = 0;

float UpdateDeltaTime()
{
    currentTime = SDL_GetTicks();
    deltaTime = (double)(currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    return deltaTime;
}