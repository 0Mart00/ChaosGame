#include "raylib.h"
#include "modules.h"

void Module_Mods_Draw(GameState *currentState) {
    static const Color MY_GRAY = { 130, 130, 130, 255 };

    DrawText("MODS & PLUGINS SYSTEM", 100, 100, 20, RAYWHITE);
    DrawText("No mods loaded.", 100, 150, 20, MY_GRAY);

    if (DrawButton("< BACK", 100, 500)) {
        *currentState = STATE_MAIN_MENU;
    }
}
