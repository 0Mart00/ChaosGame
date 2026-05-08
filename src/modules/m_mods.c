#include "modules.h"
#include "raylib.h"

void Module_Mods_Draw(GameState *currentState) {
    ClearBackground(BLACK);
    DrawText("MODS & PLUGINS", 100, 100, 30, WHITE);

    // Az új, Rectangle alapú hívás: { x, y, width, height }
    if (DrawButton((Rectangle){ 100, 500, 150, 40 }, "< BACK")) {
        *currentState = STATE_MAIN_MENU;
    }
}
