#include "hud.h"
#include "raylib.h"
#include "buildings.h"
#include "modules.h"


extern int selectedBuildingIndex;
extern BuildingSystem gameBuildings;

void DrawBottomHUD(GameState *state) {
    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    
    // Reszponzív méretek
    float hudH = screenH * 0.15f; 
    float hudY = screenH - hudH;
    float padding = 10.0f;

    // Háttér panel minden állapotban látszik
    DrawRectangleRec((Rectangle){ 0, hudY, screenW, hudH }, (Color){ 25, 25, 30, 240 });
    DrawLineEx((Vector2){ 0, hudY }, (Vector2){ screenW, hudY }, 2.0f, SKYBLUE);
    if (selectedBuildingIndex != -1) {
        // ÉPÜLET AKCIÓ MENÜ
        Building* b = &gameBuildings.buildings[selectedBuildingIndex];
        
        DrawText(TextFormat("SELECTED: %s (HP: %d/%d)", b->name, b->health, b->maxHealth), 
                 padding, hudY + 5, 15, GOLD);

        float bW = 150;
        float bH = 40;
        
        // UPGRADE GOMB
        if (DrawButton((Rectangle){ 10, hudY + 25, bW, bH }, "UPGRADE")) {
            UpgradeBuilding(&gameBuildings, selectedBuildingIndex);
        }

        // DESTROY GOMB
        if (DrawButton((Rectangle){ 20 + bW, hudY + 25, bW, bH }, "DESTROY")) {
            DestroyBuilding(&gameBuildings, selectedBuildingIndex);
            selectedBuildingIndex = -1; // Rombolás után töröljük a kijelölést
        }
        
        // DESELECT GOMB
        if (DrawButton((Rectangle){ 30 + bW * 2, hudY + 25, bW, bH }, "CLOSE")) {
            selectedBuildingIndex = -1;
        }
        } else {
        switch (*state) {
            case STATE_GAMEPLAY: {
                // ALAP HUD: Menüválasztók
                float bW = (screenW - (padding * 4)) / 3.0f;
                float bH = hudH * 0.5f;
                float bY = hudY + (hudH - bH) / 2.0f;

                if (DrawButton((Rectangle){ padding, bY, bW, bH }, "BUILDING MENU")) {
                    *state = STATE_BUILD_MENU;
                }
                if (DrawButton((Rectangle){ padding * 2 + bW, bY, bW, bH }, "SKILL TREE")) {
                    *state = STATE_SKILL_TREE;
                }
                if (DrawButton((Rectangle){ padding * 3 + bW * 2, bY, bW, bH }, "TECH TREE")) {
                    *state = STATE_TECH_TREE;
                }
                break;
            }

            case STATE_BUILD_MENU: {
                // ÉPÍTÉSI MENÜ: Dinamikus gombok az összes épülettípushoz
                int btnCount = BLD_COUNT;
                int cols = (btnCount / 2) + (btnCount % 2); // Két sorba rendezzük
                float bW = (screenW - (padding * (cols + 2))) / (cols + 1); // +1 hely a BACK gombnak
                float bH = (hudH - (padding * 3)) / 2.0f;

                // 1. BACK GOMB (Vissza a sima gameplay HUD-hoz)
                if (DrawButton((Rectangle){ padding, hudY + padding, bW, hudH - padding * 2 }, "BACK")) {
                    *state = STATE_GAMEPLAY;
                }

                // 2. ÉPÜLET GOMBOK
                for (int i = 0; i < btnCount; i++) {
                    int col = i % cols;
                    int row = i / cols;

                    float bX = (padding * 2 + bW) + col * (bW + padding);
                    float bY = hudY + padding + row * (bH + padding);

                    if (DrawButton((Rectangle){ bX, bY, bW, bH }, GetBuildingName(i))) {
                        SetPlacementMode(i); // Beállítjuk az építendő típust
                        *state = STATE_GAMEPLAY; // Visszaváltunk a játéktérre, hogy lássuk, hova rakjuk
                    }
                }
                break;
            }

            default:
                // Ha más állapotban vagyunk (pl. Skill Tree), egy sima vissza gomb jól jöhet
                if (DrawButton((Rectangle){ padding, hudY + padding, 100, hudH - padding * 2 }, "CLOSE")) {
                    *state = STATE_GAMEPLAY;
                }
                break;
        }
    }
}
