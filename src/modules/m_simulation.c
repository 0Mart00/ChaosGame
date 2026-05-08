#include "raylib.h"
#include "simulation.h"
#include <stdlib.h>

static EntitySystem entities;

void InitSimulation(void) {
    entities.count = 5000;
    for (int i = 0; i < entities.count; i++) {
        entities.x[i] = GetRandomValue(0, GetScreenWidth());
        entities.y[i] = GetRandomValue(0, GetScreenHeight());
        entities.vx[i] = GetRandomValue(-100, 100) / 50.0f;
        entities.vy[i] = GetRandomValue(-100, 100) / 50.0f;
        entities.color[i] = (Color){ GetRandomValue(50, 255), GetRandomValue(50, 255), 255, 255 };
        entities.active[i] = true;
    }
}

void UpdateSimulation(float deltaTime) {
    for (int i = 0; i < entities.count; i++) {
        if (!entities.active[i]) continue;

        // Mozgás alkalmazása
        entities.x[i] += entities.vx[i];
        entities.y[i] += entities.vy[i];

        // Egyszerű fal-ütközés (visszapattanás)
        if (entities.x[i] < 0 || entities.x[i] > GetScreenWidth()) entities.vx[i] *= -1;
        if (entities.y[i] < 0 || entities.y[i] > GetScreenHeight()) entities.vy[i] *= -1;
    }
}

void DrawSimulation(void) {
    for (int i = 0; i < entities.count; i++) {
        if (!entities.active[i]) continue;
        
        // Modern, minimalista pontok (kis körök vagy pixelpontok)
        DrawCircle((int)entities.x[i], (int)entities.y[i], 1.5f, entities.color[i]);
    }
    DrawFPS(10, 10);
    DrawText("ENTITIES: 5000+", 10, 30, 20, LIME);
}

// Ez a függvény köti össze a menüt a játékkal
void Module_Simulation_Draw(GameState *currentState) {
    static bool initialized = false;
    if (!initialized) {
        InitSimulation();
        initialized = true;
    }

    UpdateSimulation(GetFrameTime());
    DrawSimulation();

    // Kilépés a szimulációból (pl. ESC-re vissza a menübe)
    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentState = STATE_MAIN_MENU;
        initialized = false; // Reset, hogy legközelebb újragenerálja
    }
}
