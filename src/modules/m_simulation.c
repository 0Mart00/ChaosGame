#include "raylib.h"
#include "raymath.h"
#include "simulation.h"
#include "modules.h"
#include "map_system.h"
#include "overlay.h"
#include "hud.h"
#include <stdlib.h>
#include <math.h>

static EntitySystem entities;
static Camera2D gameCamera = { 0 }; // <--- EZ HIÁNYZOTT: Kamera deklarálása
static Vector2 selectorStart = { 0 };
static bool isSelecting = false;
static bool isDragging = false;
void SpecialAbility_MindControl(Rectangle area);
void HandlePlayerInput(Camera2D camera);

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

        if (entities.hasTarget[i]) {
            float dx = entities.targetX[i] - entities.x[i];
            float dy = entities.targetY[i] - entities.y[i];
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 5.0f) {
                // Egyszerű steering (kormányzás)
                entities.vx[i] = (dx / dist) * 150.0f * deltaTime;
                entities.vy[i] = (dy / dist) * 150.0f * deltaTime;
            } else {
                entities.hasTarget[i] = false;
                entities.vx[i] = 0;
                entities.vy[i] = 0;
            }
        }

        float friction = GetFrictionAt(entities.x[i], entities.y[i]);
        entities.x[i] += entities.vx[i] * friction;
        entities.y[i] += entities.vy[i] * friction;
    }
}
void DrawSimulation(void) {
    for (int i = 0; i < entities.count; i++) {
        if (!entities.active[i]) continue;

        // Alapértelmezett szín a csapat alapján
        Color drawColor = entities.color[i];

        // Ha ki van jelölve, felülbíráljuk zöldre
        if (entities.isSelected[i]) {
            drawColor = GREEN;
            // Opcionális: egy kis kör az egység alatt a kijelölés jelzésére
            DrawCircleLines((int)entities.x[i], (int)entities.y[i], 4.0f, GREEN);
        }

        DrawCircle((int)entities.x[i], (int)entities.y[i], 2.0f, drawColor);
    }
}

void Module_Simulation_Draw(GameState *currentState) {
    static bool initialized = false;
    HandlePlayerInput(gameCamera);
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

        // --- KERET RAJZOLÁSA A VILÁGBAN ---
        if (isDragging) {
            Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), gameCamera);
            Rectangle selRect = {
                fminf(selectorStart.x, mouseWorld.x),
                fminf(selectorStart.y, mouseWorld.y),
                fabsf(selectorStart.x - mouseWorld.x),
                fabsf(selectorStart.y - mouseWorld.y)
            };

            // Csak ha ténylegesen húzzuk
            if (Vector2Distance(selectorStart, mouseWorld) > 2.0f) {
                // Kitöltés (áttetsző zöld)
                DrawRectangleRec(selRect, (Color){ 0, 255, 0, 50 }); 
                // Körvonal (vastagság korrigálva a zoom-hoz)
                DrawRectangleLinesEx(selRect, 2.0f / gameCamera.zoom, LIME);
            }
        }
    EndMode2D();
    
    // Fix elemek (Overlay és HUD)
    DrawTechnicalOverlay(entities.count);
    DrawBottomHUD(currentState);
    DrawMinimapExtended(gameCamera, &entities);
    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentState = STATE_MAIN_MENU;
        initialized = false;
    }
}

void HandlePlayerInput(Camera2D camera) {
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    
    // --- 1. BAL EGÉRGOMB LENYOMÁSA (Kijelölés kezdete) ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Megnézzük, van-e már kijelölt egységünk
        bool anySelected = false;
        for (int i = 0; i < entities.count; i++) {
            if (entities.active[i] && entities.team[i] == 0 && entities.isSelected[i]) {
                anySelected = true;
                break;
            }
        }

        // Ha van kijelölve valami, akkor a kattintás MOZGATÁS parancs
        if (anySelected && !isDragging) {
            for (int i = 0; i < entities.count; i++) {
                if (entities.isSelected[i]) {
                    entities.targetX[i] = mouseWorld.x;
                    entities.targetY[i] = mouseWorld.y;
                    entities.hasTarget[i] = true;
                }
            }
        }

        // Elindítjuk a kijelölő keret rajzolását is
        selectorStart = mouseWorld;
        isDragging = true;
    }

    // --- 2. NYOMVATARTÁS ÉS FELENGEDÉS ---
    if (isDragging) {
        Rectangle selRect = {
            fminf(selectorStart.x, mouseWorld.x),
            fminf(selectorStart.y, mouseWorld.y),
            fabsf(selectorStart.x - mouseWorld.x),
            fabsf(selectorStart.y - mouseWorld.y)
        };

        // Csak akkor rajzolunk keretet, ha ténylegesen húzzuk az egeret (nem csak egy kattintás)
        if (Vector2Distance(selectorStart, mouseWorld) > 5.0f) {
            DrawRectangleLinesEx(selRect, 1.0f / camera.zoom, LIME);
            DrawRectangleRec(selRect, (Color){ 0, 255, 0, 30 }); // Áttetsző zöld kitöltés
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Ha a húzás nagyobb volt mint egy küszöbérték, újrakalkuláljuk a kijelölést
            if (Vector2Distance(selectorStart, mouseWorld) > 5.0f) {
                for (int i = 0; i < entities.count; i++) {
                    if (entities.active[i] && entities.team[i] == 0) {
                        entities.isSelected[i] = CheckCollisionPointRec(
                            (Vector2){entities.x[i], entities.y[i]}, selRect);
                    }
                }
            }
            isDragging = false;
        }
    }

    // --- KIJELÖLÉS (Bal egérgomb) ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectorStart = mouseWorld;
        isSelecting = true;
    }

    if (isSelecting) {
        Rectangle selRect = {
            fminf(selectorStart.x, mouseWorld.x),
            fminf(selectorStart.y, mouseWorld.y),
            fabsf(selectorStart.x - mouseWorld.x),
            fabsf(selectorStart.y - mouseWorld.y)
        };

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < entities.count; i++) {
                if (entities.active[i] && entities.team[i] == 0) {
                    entities.isSelected[i] = CheckCollisionPointRec((Vector2){entities.x[i], entities.y[i]}, selRect);
                }
            }
            isSelecting = false;
        }
        
        // Vizuális visszacsatolás a keretről
        DrawRectangleLinesEx(selRect, 1.0f / camera.zoom, GREEN);
    }

    // --- MOZGATÁS (Jobb egérgomb) ---
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        for (int i = 0; i < entities.count; i++) {
            if (entities.isSelected[i]) {
                entities.targetX[i] = mouseWorld.x;
                entities.targetY[i] = mouseWorld.y;
                entities.hasTarget[i] = true;
            }
        }
    }

    // --- SPECIÁLIS KÉPESSÉG (Például: Q billentyű) ---
    if (IsKeyPressed(KEY_Q)) {
        // Egy 100x100-as terület az egér körül
        Rectangle abilityArea = { mouseWorld.x - 50, mouseWorld.y - 50, 100, 100 };
        SpecialAbility_MindControl(abilityArea);
        
        // Vizuális visszacsatolás (opcionális: villanás)
        DrawRectangleLinesEx(abilityArea, 2.0f, PURPLE); 
    }
}
void SpecialAbility_MindControl(Rectangle area) {
    for (int i = 0; i < entities.count; i++) {
        if (!entities.active[i]) continue;
        if (CheckCollisionPointRec((Vector2){entities.x[i], entities.y[i]}, area)) {
            entities.team[i] = 0;
            entities.color[i] = SKYBLUE;
        }
    }
}


