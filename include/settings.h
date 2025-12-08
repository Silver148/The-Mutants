#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct{
    int volume_music_in_game;
    int volume_music_in_menu;
} Settings;

int LoadSettingsDefault(Settings* settings);
int LoadSettingsFromFile(Settings* settings);
int SaveSettings(Settings* settings);

#endif // SETTINGS_H