/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: settings.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <shlobj.h>
#include <direct.h>
#include "settings.h"

char settingsPATH[MAX_PATH];

void EnsurePath() {
    if (settingsPATH[0] == '\0') {
        char DocumentsPATH[MAX_PATH];
        if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, DocumentsPATH) == S_OK) {
            
            char folderPath[MAX_PATH];
            snprintf(folderPath, sizeof(folderPath), "%s\\The Mutants", DocumentsPATH);
            _mkdir(folderPath);
            snprintf(settingsPATH, sizeof(settingsPATH), "%s\\settings.cfg", folderPath);
        }
    }
}

int LoadSettingsDefault(Settings* settings) {
    if (settings == NULL) {
        return -1;
    }
    settings->volume_music_in_game = 50;
    settings->volume_music_in_menu = 50;
    settings->fullscreen = false;
    return 0;
}

int SaveSettings(Settings* settings){

    EnsurePath();

    FILE* file = fopen(settingsPATH, "w");
    if(!file) return -1;

    fprintf(file, "volume_music_in_game=%d\n", settings->volume_music_in_game);
    fprintf(file, "volume_music_in_menu=%d\n", settings->volume_music_in_menu);
    fprintf(file, "fx=%d\n",settings->fx);
    fprintf(file, "fullscreen=%d\n",settings->fullscreen);

    fclose(file);

    return 0;
}

int LoadSettingsFromFile(Settings* settings) {

    EnsurePath();

    FILE* file = fopen(settingsPATH, "r");
    if(!file) {
        SDL_Log("Creating settings.cfg with default configurations\n");
        LoadSettingsDefault(settings);
        SaveSettings(settings);
        return 0;
    }
    
    char line[128];
    while(fgets(line, sizeof(line), file)) {
        char key[64], value[64];

        if(sscanf(line, "%63[^=]=%63[^\n]", key, value) == 2) {
            if(strcmp(key, "volume_music_in_game") == 0) {
                settings->volume_music_in_game = atoi(value);
            } else if(strcmp(key, "volume_music_in_menu") == 0) {
                settings->volume_music_in_menu = atoi(value);
            }else if(strcmp(key, "fullscreen") == 0){
                settings->fullscreen = atoi(value);
            }else if(strcmp(key, "fx") == 0){
                settings->fx = atoi(value);
            }else {
                SDL_Log("Unknown setting: %s\n", key);
                break;
            }
        }

    }

    fclose(file);

    return 0;

}