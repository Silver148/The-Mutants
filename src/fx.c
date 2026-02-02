#include "fx.h"

void RenderFXInterference(SDL_Renderer* renderer) {
    if ((rand() % 100) < 5) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        SDL_Rect line = { 0, rand() % 480, 640, rand() % 3 };
        SDL_RenderFillRect(renderer, &line);
    }
}

void RenderFXNoise(SDL_Renderer* renderer) {

    for(int i = 0; i < 1000; i++){
        if(rand() % 100 < 10){
            int x = rand() % 640;
            int y = rand() % 480;
            SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 40);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}