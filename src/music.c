/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: music.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/* MUSIC MANAGEMENT by Juan Yaguaro */

#include "music.h"
#include "settings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern Settings game_settings;

int InitMusic()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    return 0;
}

int CloseMusic()
{
    Mix_HaltMusic();
    
    Mix_CloseAudio();
    return 0;
}

int PlayMusicStateGame()
{
    Mix_Music* bgm = Mix_LoadMUS("music/Terminator 2 Judgment Day _ Guns N' Roses - You Could Be Mine (Terminator 2 Soundtrack) - Braindead Soundtracks.mp3");
    if(bgm == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    Mix_VolumeMusic(game_settings.volume_music_in_game);

    Mix_PlayMusic(bgm, -1); // Play music indefinitely
    return 0;
}

int PlayMusicStateMenu()
{
    Mix_Music* bgm = Mix_LoadMUS("music/The Mutants Menu Music.mp3");
    if(bgm == NULL)
    {
        printf("Failed to load menu music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    Mix_VolumeMusic(game_settings.volume_music_in_menu);
    Mix_PlayMusic(bgm, -1); // Play music indefinitely
    return 0;
}

int ChangeMusicVolume(int volume)
{
    Mix_VolumeMusic(volume);
    return 0;
}
