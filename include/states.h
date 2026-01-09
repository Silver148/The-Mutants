#ifndef STATES_H
#define STATES_H

/*MENU*/
int Init_State_Menu();
int Update_State_Menu();

/*GAME*/
int Init_State_Game();
int Update_State_Game();
void UpdateKillsTexture(int kills);
void CleanupKillsTexture();
void GamePaused();
void CleanGamePaused();
void CleanupBackground();

#endif // STATES_H