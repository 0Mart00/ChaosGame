#include "hud.h"
#include "raylib.h"

void DrawBottomHUD(GameState *state) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int hudHeight = 80;

    // Alsó panel rajzolása
    DrawRectangle(0, screenH - hudHeight, screenW, hudHeight, (Color){15, 15, 15, 255});
    DrawLine(0, screenH - hudHeight, screenW, screenH - hudHeight, MY_CYAN);

    // Alsó menü gombjai
    if (DrawButton("SPAWN CHAOS", 20, screenH - 65)) {
        // Ide jöhet egy kreatív akció trigger
    }

    if (DrawButton("MENU", screenW - 220, screenH - 65)) {
        *state = STATE_MAIN_MENU;
    }

    DrawText("CREATIVE COMMANDS", screenW / 2 - 80, screenH - 75, 15, DARKGRAY);
}
