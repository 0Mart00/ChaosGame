#include "raylib.h"
#include "raymath.h"
#include "simulation.h"
#include "modules.h"
#include "map_system.h"
#include "overlay.h"
#include "hud.h"
#include <stdlib.h>
#include <math.h>
#include "buildings.h"


static EntitySystem entities;
static Camera2D gameCamera = { 0 }; // <--- EZ HIÁNYZOTT: Kamera deklarálása
static Vector2 selectorStart = { 0 };
static bool isSelecting = false;
static bool isDragging = false;
static int activePlacementType = -1; // -1 = nincs építés alatt semmi
static bool isPlacementMode = false;
                                   
void SpecialAbility_MindControl(Rectangle area);
void HandleBuildingSelection(Vector2 mouseWorld);
void HandlePlayerInput(Camera2D camera, GameState *state);
BuildingSystem gameBuildings;
int selectedBuildingIndex = -1;

                                      
void Module_Simulation_Update(float deltaTime) {
    // Egységek frissítése
    UpdateSimulation(deltaTime);
    
    // --- ÉPÜLETEK FRISSÍTÉSE ---
    UpdateBuildings(&gameBuildings, deltaTime);
}

void InitSimulation(void) {
    InitBuildingSystem(&gameBuildings); // Ez üríti ki a listát és állítja nullára a számlálót
                                        //
                                        //
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
    // Pálya határai pixelben
    float worldMaxX = (float)(MAP_WIDTH * TILE_SIZE);
    float worldMaxY = (float)(MAP_HEIGHT * TILE_SIZE);

    for (int i = 0; i < entities.count; i++) {
        if (!entities.active[i]) continue;

        if (entities.hasTarget[i]) {
            float dx = entities.targetX[i] - entities.x[i];
            float dy = entities.targetY[i] - entities.y[i];
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 5.0f) {
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

        // --- HATÁROK KÉNYSZERÍTÉSE (BOUNDARY CLAMP) ---
        // X tengely ellenőrzése
        if (entities.x[i] < 0) {
            entities.x[i] = 0;
            entities.vx[i] = 0;
        } else if (entities.x[i] > worldMaxX) {
            entities.x[i] = worldMaxX;
            entities.vx[i] = 0;
        }

        // Y tengely ellenőrzése
        if (entities.y[i] < 0) {
            entities.y[i] = 0;
            entities.vy[i] = 0;
        } else if (entities.y[i] > worldMaxY) {
            entities.y[i] = worldMaxY;
            entities.vy[i] = 0;
        }
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
        if (selectedBuildingIndex != -1) {
            Building* b = &gameBuildings.buildings[selectedBuildingIndex];
            DrawRectangleLinesEx(b->bounds, 3.0f, GOLD); // Arany keret a kijelölt épület köré
        }
        DrawCircle((int)entities.x[i], (int)entities.y[i], 2.0f, drawColor);
    }
}

void Module_Simulation_Draw(GameState *currentState) {
    static bool initialized = false;
    // 1. Inicializálás (Csak egyszer fut le)
    if (!initialized) {
        InitSimulation();
        InitMapSystem("ChaosSeed123");
        InitBuildingSystem(&gameBuildings); // Ne felejtsd el az épületeket is!
        initialized = true;
    }

    HandlePlayerInput(gameCamera, currentState);



    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        gameCamera.target.x -= delta.x / gameCamera.zoom;
        gameCamera.target.y -= delta.y / gameCamera.zoom;
    }
    
    gameCamera.zoom += ((float)GetMouseWheelMove() * 0.05f);
    gameCamera.zoom = Clamp(gameCamera.zoom, 0.5f, 3.0f);

    // Kamera korlátozás (Clamp logika - marad a tiéd)
    float viewW = (float)GetScreenWidth() / gameCamera.zoom;
    float viewH = (float)(GetScreenHeight() - 80) / gameCamera.zoom;
    float minX = viewW / 2.0f;
    float minY = viewH / 2.0f;
    float maxX = (MAP_WIDTH * TILE_SIZE) - (viewW / 2.0f);
    float maxY = (MAP_HEIGHT * TILE_SIZE) - (viewH / 2.0f);

    if (viewW >= (MAP_WIDTH * TILE_SIZE)) gameCamera.target.x = (MAP_WIDTH * TILE_SIZE) / 2.0f;
    else gameCamera.target.x = Clamp(gameCamera.target.x, minX, maxX);

    if (viewH >= (MAP_HEIGHT * TILE_SIZE)) gameCamera.target.y = (MAP_HEIGHT * TILE_SIZE) / 2.0f;
    else gameCamera.target.y = Clamp(gameCamera.target.y, minY, maxY);

    // --- RAJZOLÁS ---
    
    // A játéktér rajzolása: Gameplay ÉS Build Menu alatt is látni akarjuk!
    if (*currentState == STATE_GAMEPLAY || *currentState == STATE_BUILD_MENU) {
        
        BeginMode2D(gameCamera); 
            // 1. Térkép és Egységek
            DrawMap(gameCamera);
            DrawBuildings(&gameBuildings); // Épületek megjelenítése
            DrawSimulation();
            
            // 2. Logika frissítése (Csak ha nem áll a játék)
            Module_Simulation_Update(GetFrameTime());

            // 3. Kijelölő keret (isDragging)
            if (isDragging) {
                Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), gameCamera);
                Rectangle selRect = {
                    fminf(selectorStart.x, mouseWorld.x), fminf(selectorStart.y, mouseWorld.y),
                    fabsf(selectorStart.x - mouseWorld.x), fabsf(selectorStart.y - mouseWorld.y)
                };
                DrawRectangleRec(selRect, (Color){ 0, 255, 0, 50 });
                DrawRectangleLinesEx(selRect, 2.0f / gameCamera.zoom, LIME);
            }

            // 4. Épület "Szellem" (Placement Mode)
            if (isPlacementMode) {
                Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), gameCamera);
                // 64x64-es keret a kurzor körül
                DrawRectangleLinesEx((Rectangle){ mouseWorld.x - 32, mouseWorld.y - 32, 64, 64 }, 2.0f, LIME);
            }
        EndMode2D();
    }

    // Fix UI elemek (A kamera után, hogy ne mozogjanak el)
    DrawTechnicalOverlay(entities.count);
    DrawBottomHUD(currentState); // Ez rajzolja ki a gombokat a state alapján
    DrawMinimapExtended(gameCamera, &entities);

    // Kilépés kezelése
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (*currentState == STATE_BUILD_MENU) {
            *currentState = STATE_GAMEPLAY; // Menüből vissza a játékba
        } else {
            *currentState = STATE_MAIN_MENU; // Játékból vissza a főmenübe
            initialized = false;
        }
    }
}
void HandlePlayerInput(Camera2D camera, GameState *state) {
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isPlacementMode) {
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        HandleBuildingSelection(mouseWorld);
    }
    if (*state == STATE_SKILL_TREE || *state == STATE_TECH_TREE || *state  == STATE_BUILD_MENU) {
        return; 
    }
    if (isPlacementMode) {
        // 1. Vizualizáció: Rajzoljuk ki az épület "szellemét" (Ghost building)
        Color ghostColor = (Color){ 0, 255, 0, 150 }; // Áttetsző zöld
        DrawRectangleLinesEx((Rectangle){ mouseWorld.x - 32, mouseWorld.y - 32, 64, 64 }, 2.0f, ghostColor);

        // 2. Épület lerakása (Bal klikk)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Itt hívjuk meg a korábban létrehozott Building System-et
            PlaceBuilding(&gameBuildings, activePlacementType, (Vector2){ mouseWorld.x - 32, mouseWorld.y - 32 }, 0);
            
            // Ha csak egyet akarunk építeni, kilépünk az módból:
            isPlacementMode = false; 
            activePlacementType = -1;
        }

        // 3. Mégse (Jobb klikk)
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            isPlacementMode = false;
            activePlacementType = -1;
        }
    }
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

void SetPlacementMode(int buildingType) {
    activePlacementType = buildingType;
    isPlacementMode = true;
 }

void HandleBuildingSelection(Vector2 mouseWorld) {
    selectedBuildingIndex = -1; // Alaphelyzetben töröljük a kijelölést
    
    for (int i = 0; i < gameBuildings.count; i++) {
        if (!gameBuildings.buildings[i].isActive) continue;
        
        if (CheckCollisionPointRec(mouseWorld, gameBuildings.buildings[i].bounds)) {
            selectedBuildingIndex = i;
            break;
        }
    }
}

