#ifndef STATES_H
#define STATES_H

typedef enum{
    STATE_GAME,
    STATE_MENU,
    STATE_EXIT
} STATES;

/*MENU*/
int Init_State_Menu();
int Update_State_Menu();

/*GAME*/
int Init_State_Game();
int Update_State_Game();
void UpdateKillsTexture(int kills);
void UpdateAmmunitions();
void CleanupKillsTexture();
void CleanupAmmunitions();
void GamePaused();
void CleanGamePaused();
void CleanupBackground();

#endif // STATES_H