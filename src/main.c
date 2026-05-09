#include "raylib.h"
#include "modules.h"
#include "simulation.h"
#include "network_system.h"



Font chaosFont;

int main(void) {
    InitWindow(800, 600, "Chaos RTS - Modular Menu");
    chaosFont = LoadFontEx("assets/fonts/inter.ttf", 32, 0, 250);
    if (!InitNetwork()) return 1; // Hálózat inicializálása
    GameState currentState = STATE_MAIN_MENU;
    SetTargetFPS(60);

    while (!WindowShouldClose() && currentState != STATE_QUIT) {
        UpdateNetwork(); // Hálózati események pollolása
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case STATE_MAIN_MENU:
                // A gombokat egy Rectangle struktúrába csomagoljuk: { x, y, szélesség, magasság }
                float centerX = GetScreenWidth() / 2.0f - 100; // 200 széles gomb fele az eltolás

                if (DrawButton((Rectangle){ centerX, 200, 200, 50 }, "MULTIPLAYER")) {
                    currentState = STATE_MULTIPLAYER;
                }
    
                if (DrawButton((Rectangle){ centerX, 270, 200, 50 }, "SETTINGS")) {
                    currentState = STATE_SETTINGS;
                }
    
                if (DrawButton((Rectangle){ centerX, 340, 200, 50 }, "MODS & PLUGINS")) {
                    currentState = STATE_MODS;
                }
    
                if (DrawButton((Rectangle){ centerX, 410, 200, 50 }, "QUIT")) {
                    currentState = STATE_QUIT;
                }
                break;

            case STATE_SETTINGS:
                Module_Settings_Draw(&currentState);
                break;

            case STATE_MULTIPLAYER:
                Module_Multiplayer_Draw(&currentState);
                break;

            case STATE_MODS:
                Module_Mods_Draw(&currentState);
                break;

            case STATE_GAMEPLAY:
            case STATE_BUILD_MENU:
            case STATE_SKILL_TREE:
            case STATE_TECH_TREE:
                Module_Simulation_Draw(&currentState);
                break;
            case STATE_QUIT:
                // A hurok feltétele kiléptet, de a switch-be is kell a warning miatt
                break;
        }

        EndDrawing();
    }
    CleanupNetwork();
    CloseWindow();
    return 0;
}
