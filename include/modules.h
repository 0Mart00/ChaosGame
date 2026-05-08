#ifndef MODULES_H
#define MODULES_H

#include "raylib.h"  // <--- EZ KELL AZ ELEJÉRE a Color típus miatt
#include <stdbool.h> // Szükséges a bool típushoz

static const Color MY_CYAN = { 0, 255, 255, 255 };
static const Color MY_GRAY = { 130, 130, 130, 255 };


typedef enum {
    STATE_MAIN_MENU,
    STATE_MULTIPLAYER,
    STATE_SETTINGS,
    STATE_MODS,
    STATE_GAMEPLAY,
    STATE_QUIT,
    STATE_BUILD_MENU,
    STATE_SKILL_TREE,
    STATE_TECH_TREE,
} GameState;

// Segédfüggvény a menühöz
bool DrawButton(Rectangle bounds, const char* text);


// Modulok
void Module_Settings_Draw(GameState *currentState);
void Module_Multiplayer_Draw(GameState *currentState);
void Module_Mods_Draw(GameState *currentState);
void HandleTextInput(char* buffer, int maxSize);


#endif
