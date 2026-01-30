#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* world barrier width (used by multiple modules) */
#define WORLD_BARRIER_WIDTH 64

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int worldBarrierX; /* world-space X coordinate of right-side barrier */
extern int worldBarrierLeftX; /* world-space X coordinate of left-side barrier */

/* Player spawn coordinates (world space) set at game init) */
extern float player_spawn_x;
extern float player_spawn_y;

#endif // GLOBAL_VARS_H