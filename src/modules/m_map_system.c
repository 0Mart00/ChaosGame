#include "raylib.h"
#include "map_system.h"
#include <string.h>
#include "modules.h"

static GameMap map;

// PCG Seed Generátor: FNV-1a Hash (String -> uint64_t)
uint64_t GenerateSeed(const char* str) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

void InitMapSystem(const char* seedString) {
    map.seed = GenerateSeed(seedString);
    
    // Perlin zaj generálása a Raylib Image API-val
    // ÚJ (Javított):
    // Alternatív megoldás: Véletlenszerű zaj generálása és elmosása
    Image noise = GenImageWhiteNoise(MAP_WIDTH, MAP_HEIGHT, 0.5f);
    ImageBlurGaussian(&noise, 5); // Elmosás, hogy összefüggő területeket kapjunk
    Color *pixels = LoadImageColors(noise);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float v = pixels[y * MAP_WIDTH + x].r / 255.0f; // 0.0 - 1.0 tartomány
            
            if (v < 0.35f) {
                map.tiles[x][y] = (Tile){BIOME_WATER, 0.2f, (Color){20, 40, 80, 255}};
            } else if (v < 0.45f) {
                map.tiles[x][y] = (Tile){BIOME_SAND, 0.8f, (Color){190, 160, 110, 255}};
            } else if (v < 0.75f) {
                map.tiles[x][y] = (Tile){BIOME_GRASS, 1.0f, (Color){30, 70, 40, 255}};
            } else {
                map.tiles[x][y] = (Tile){BIOME_MOUNTAIN, 0.5f, (Color){60, 60, 70, 255}};
            }
        }
    }

    UnloadImageColors(pixels);
    UnloadImage(noise);

    // Minimap textúra előkészítése
    map.minimapTexture = LoadRenderTexture(MAP_WIDTH, MAP_HEIGHT);
    BeginTextureMode(map.minimapTexture);
    ClearBackground(BLACK);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            DrawPixel(x, y, map.tiles[x][y].color);
        }
    }
    EndTextureMode();
}

void DrawMap(Camera2D camera) {
    // Csak a látható rész kirajzolása (Culling)
    int startX = (int)((camera.target.x - GetScreenWidth()/2) / TILE_SIZE) - 1;
    int startY = (int)((camera.target.y - GetScreenHeight()/2) / TILE_SIZE) - 1;
    int endX = startX + (GetScreenWidth() / TILE_SIZE) + 2;
    int endY = startY + (GetScreenHeight() / TILE_SIZE) + 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > MAP_WIDTH) endX = MAP_WIDTH;
    if (endY > MAP_HEIGHT) endY = MAP_HEIGHT;

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, map.tiles[x][y].color);
            // Kognitív rácsháló a technikai érzetért
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){0,0,0,30});
        }
    }
}

void DrawMinimap(void) {
    int margin = 20;
    int size = 150;
    Rectangle dest = { (float)(GetScreenWidth() - size - margin), (float)margin, (float)size, (float)size };
    
    DrawRectangle(dest.x - 2, dest.y - 2, dest.width + 4, dest.height + 4, MY_CYAN); // Keret
    // RenderTexture y-tengelye fordított, ezért negatív magasságot adunk meg
    DrawTexturePro(map.minimapTexture.texture, 
                   (Rectangle){0, 0, MAP_WIDTH, -MAP_HEIGHT}, 
                   dest, (Vector2){0,0}, 0, WHITE);
}

float GetFrictionAt(float worldX, float worldY) {
    int x = (int)(worldX / TILE_SIZE);
    int y = (int)(worldY / TILE_SIZE);
    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
        return map.tiles[x][y].friction;
    }
    return 1.0f;
}

void CleanupMapSystem(void) {
    UnloadRenderTexture(map.minimapTexture);
}

void DrawMinimapExtended(Camera2D camera) {
    int margin = 20;
    int size = 150;
    Rectangle dest = { (float)(GetScreenWidth() - size - margin), (float)margin, (float)size, (float)size };
    
    // 1. Térkép textúra rajzolása
    DrawRectangleRec((Rectangle){dest.x - 2, dest.y - 2, dest.width + 4, dest.height + 4}, DARKGRAY);
    DrawTexturePro(map.minimapTexture.texture, 
                   (Rectangle){0, 0, MAP_WIDTH, -MAP_HEIGHT}, 
                   dest, (Vector2){0,0}, 0, WHITE);
    DrawRectangleLinesEx(dest, 1, MY_CYAN);

    // 2. KAMERA KERET A MINIMAPON
    // Kiszámoljuk a kamera helyzetét arányosan a minimap méretéhez képest
    float worldToMinimap = (float)size / (MAP_WIDTH * TILE_SIZE);
    
    float viewW = GetScreenWidth() / camera.zoom;
    float viewH = (GetScreenHeight() - 80) / camera.zoom;

    float camMinimapX = (camera.target.x - viewW / 2.0f) * worldToMinimap;
    float camMinimapY = (camera.target.y - viewH / 2.0f) * worldToMinimap;
    float camMinimapW = viewW * worldToMinimap;
    float camMinimapH = viewH * worldToMinimap;

    Rectangle camRect = { 
        dest.x + camMinimapX, 
        dest.y + camMinimapY, 
        camMinimapW, 
        camMinimapH 
    };

    DrawRectangleLinesEx(camRect, 1, LIME);
}
