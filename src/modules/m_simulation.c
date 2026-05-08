#include "raylib.h"
#include "simulation.h"
#include <stdlib.h>
#include "map_system.h"


static EntitySystem entities;
static Camera2D gameCamera = { 0 }; // <--- EZ HIÁNYZOTT: Kamera deklarálása
                                    //
                                    //
void InitSimulation(void) {
    // Kamera alaphelyzetbe állítása
    gameCamera.target = (Vector2){ 0, 0 };
    gameCamera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    gameCamera.rotation = 0.0f;
    gameCamera.zoom = 1.0f;

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

        // Súrlódás lekérése a talajról
        float friction = GetFrictionAt(entities.x[i], entities.y[i]);

        entities.x[i] += entities.vx[i] * friction;
        entities.y[i] += entities.vy[i] * friction;
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

void Module_Simulation_Draw(GameState *currentState) {
    static bool initialized = false;
    if (!initialized) {
        InitSimulation();
        InitMapSystem("ChaosSeed123"); // Térkép inicializálása
        initialized = true;
    }

    // Kamera mozgatás (egérrel vagy nyilakkal)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        gameCamera.target.x -= delta.x / gameCamera.zoom;
        gameCamera.target.y -= delta.y / gameCamera.zoom;
    }
    gameCamera.zoom += ((float)GetMouseWheelMove() * 0.05f);
    if (gameCamera.zoom < 0.1f) gameCamera.zoom = 0.1f;

    BeginMode2D(gameCamera); 
        DrawMap(gameCamera);
        UpdateSimulation(GetFrameTime());
        DrawSimulation();
    EndMode2D();
    
    DrawMinimap(); // <--- JAVÍTVA: Elírás (D_qrawMinimap) eltávolítva
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentState = STATE_MAIN_MENU;
        initialized = false;
    }
}
