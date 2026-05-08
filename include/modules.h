#ifndef MODULES_H
#define MODULES_H

#include <stdbool.h> // Szükséges a bool típushoz

typedef enum {
    STATE_MAIN_MENU,
    STATE_MULTIPLAYER,
    STATE_SETTINGS,
    STATE_MODS,
    STATE_GAMEPLAY,
    STATE_QUIT
} GameState;

// Segédfüggvény a menühöz
bool DrawButton(const char* text, int x, int y);

// Modulok
void Module_Settings_Draw(GameState *currentState);
void Module_Multiplayer_Draw(GameState *currentState);
void Module_Mods_Draw(GameState *currentState);
void HandleTextInput(char* buffer, int maxSize);
#endif
