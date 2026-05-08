#ifndef MAP_SYSTEM_H
#define MAP_SYSTEM_H

#include "raylib.h"
#include <stdint.h>

#define MAP_WIDTH 256
#define MAP_HEIGHT 256
#define TILE_SIZE 32  // A vizuális megjelenítéshez

struct EntitySystem;


typedef enum {
    BIOME_WATER,
    BIOME_SAND,
    BIOME_GRASS,
    BIOME_MOUNTAIN
} BiomeType;

typedef struct {
    BiomeType type;
    float friction;
    Color color;
} Tile;

typedef struct {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
    uint64_t seed;
    RenderTexture2D minimapTexture;
} GameMap;

// Függvények
void InitMapSystem(const char* seedString);
void DrawMap(Camera2D camera);
void DrawMinimap(void);
void DrawMinimapExtended(Camera2D camera, struct EntitySystem *entities);
float GetFrictionAt(float worldX, float worldY);
void CleanupMapSystem(void);

#endif
