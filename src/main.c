#include "raylib.h"
#include "modules.h"
#include "simulation.h"
#include "network_system.h"


int main(void) {
    InitWindow(800, 600, "Chaos RTS - Modular Menu");
    if (!InitNetwork()) return 1; // Hálózat inicializálása
    GameState currentState = STATE_MAIN_MENU;
    SetTargetFPS(60);

    while (!WindowShouldClose() && currentState != STATE_QUIT) {
        UpdateNetwork(); // Hálózati események pollolása
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case STATE_MAIN_MENU:
                DrawText("CHAOS RTS", 300, 100, 40, RAYWHITE);
                if (DrawButton("MULTIPLAYER", 300, 200)) currentState = STATE_MULTIPLAYER;
                if (DrawButton("SETTINGS", 300, 270))    currentState = STATE_SETTINGS;
                if (DrawButton("MODS & PLUGINS", 300, 340)) currentState = STATE_MODS;
                if (DrawButton("QUIT", 300, 410))        currentState = STATE_QUIT;
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
