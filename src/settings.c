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
            snprintf(settingsPATH, sizeof(settingsPATH), "%s\\settings.data", folderPath);
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

    FILE* file = fopen(settingsPATH, "wb");
    if(!file) return -1;

    fwrite(settings, sizeof(Settings), 1, file);

    fclose(file);

    return 0;
}

int LoadSettingsFromFile(Settings* settings) {

    EnsurePath();

    FILE* file = fopen(settingsPATH, "r");
    if(!file) {
        SDL_Log("Creating settings.data with default configurations\n");
        LoadSettingsDefault(settings);
        SaveSettings(settings);
        return 0;
    }

    fread(settings, sizeof(Settings), 1, file);

    fclose(file);

    return 0;

}