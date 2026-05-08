#include "raylib.h"
#include "simulation.h"
#include "modules.h"
#include "map_system.h"
#include "overlay.h"
#include "hud.h"
#include <stdlib.h>

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
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        gameCamera.target.x -= delta.x / gameCamera.zoom;
        gameCamera.target.y -= delta.y / gameCamera.zoom;
    }
   
    // 2. Zoom kezelése
    gameCamera.zoom += ((float)GetMouseWheelMove() * 0.05f);
    if (gameCamera.zoom < 0.5f) gameCamera.zoom = 0.5f; // Ne lehessen túl távolra menni
    if (gameCamera.zoom > 3.0f) gameCamera.zoom = 3.0f;

    // 3. KAMERA KORLÁTOZÁSA (Clamp)
    // Kiszámoljuk a látható terület szélességét és magasságát a világ-koordinátákban
    // 1. Kiszámoljuk, mekkora területet látunk be valójában (világ koordinátákban)
    float viewW = (float)GetScreenWidth() / gameCamera.zoom;
    float viewH = (float)(GetScreenHeight() - 80) / gameCamera.zoom; // -80 a HUD magassága

    // 2. Meghatározzuk a határokat
    // A cél, hogy a kamera széle (target - view/2) ne legyen kisebb 0-nál,
    // és a másik széle (target + view/2) ne legyen nagyobb a pálya méreténél.
    float minX = viewW / 2.0f;
    float minY = viewH / 2.0f;
    float maxX = (MAP_WIDTH * TILE_SIZE) - (viewW / 2.0f);
    float maxY = (MAP_HEIGHT * TILE_SIZE) - (viewH / 2.0f);

    // 3. Alkalmazzuk a kényszerítést (Clamp)
    // Ha a pálya kisebb, mint a belátható nézet, akkor középre rakjuk
    if (viewW >= (MAP_WIDTH * TILE_SIZE)) {
        gameCamera.target.x = (MAP_WIDTH * TILE_SIZE) / 2.0f;
    } else {
        if (gameCamera.target.x < minX) gameCamera.target.x = minX;
        if (gameCamera.target.x > maxX) gameCamera.target.x = maxX;
    }

    if (viewH >= (MAP_HEIGHT * TILE_SIZE)) {
    // Itt a trükk: mivel fent lógott ki, figyelembe kell venni az eltolást!
        gameCamera.target.y = (MAP_HEIGHT * TILE_SIZE) / 2.0f;
    } else {
        if (gameCamera.target.y < minY) gameCamera.target.y = minY;
        if (gameCamera.target.y > maxY) gameCamera.target.y = maxY;
    }

    BeginMode2D(gameCamera); 
        DrawMap(gameCamera);
        UpdateSimulation(GetFrameTime());
        DrawSimulation();
    EndMode2D();
    
    // Fix elemek (Overlay és HUD)
    DrawTechnicalOverlay(entities.count);
    DrawBottomHUD(currentState);
    DrawMinimap();
    DrawMinimapExtended(gameCamera, &entities);
    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentState = STATE_MAIN_MENU;
        initialized = false;
    }
}
