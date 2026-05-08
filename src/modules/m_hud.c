#include "hud.h"
#include "raylib.h"

void DrawBottomHUD(GameState *state) {
    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    float hudH = screenH * 0.12f;
    float bW = screenW / 5.0f; // 5 gomb szélességnyi hely
    float bH = hudH * 0.6f;
    float bY = (screenH - hudH) + (hudH - bH) / 2.0f;

    // Building Menu
    if (DrawButton((Rectangle){ 10, bY, bW, bH }, "BUILDING")) {
        *state = STATE_BUILD_MENU;
    }

    // Skill Tree
    if (DrawButton((Rectangle){ 20 + bW, bY, bW, bH }, "SKILLS")) {
        *state = STATE_SKILL_TREE;
    }

    // Tech Tree
    if (DrawButton((Rectangle){ 30 + bW * 2, bY, bW, bH }, "TECH TREE")) {
        *state = STATE_TECH_TREE;
    }
}
