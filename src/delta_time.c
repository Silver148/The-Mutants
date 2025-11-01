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