#ifndef SYSTEM_CINEMATICS_H
#define SYSTEM_CINEMATICS_H

#include <SDL2/SDL.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>

void InitSystemCinematics();
int PlayCinematic(const char* filepath, SDL_Renderer* renderer);
int ShutdownCinematicsSystem();

#endif