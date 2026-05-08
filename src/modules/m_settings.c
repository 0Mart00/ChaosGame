#include "raylib.h"
#include "modules.h"

 void Module_Settings_Draw(GameState *currentState) {
    DrawText("SETTINGS", 10, 10, 20, GRAY);
    // Seed beviteli mező szimuláció
    DrawText("ENTER SEED:", 300, 200, 20, GREEN);
    DrawRectangleLines(300, 230, 200, 30, GREEN);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){ 10, 550, 100, 40 })) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) *currentState = STATE_MAIN_MENU;
    }
    DrawText("< BACK", 10, 560, 20, RED);
}
