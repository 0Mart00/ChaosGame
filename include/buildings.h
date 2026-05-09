#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    BLD_BAR,
    BLD_TEMPLE,
    BLD_POLICE_STATION,
    BLD_TOWN_HALL,
    BLD_TERRAFORMER,
    BLD_MIRAGE_TOWER,
    BLD_GRAVITY_WELL,
    BLD_HACKER_DEN,
    BLD_SCRAP_YARD,
    BLD_TRADING_POST,
    BLD_WEATHER_LAB,
    BLD_ALCHEMY_LAB,
    BLD_COUNT
} BuildingType;

typedef struct {
    BuildingType type;
    const char* name;
    int health;
    int maxHealth;
    int cost;
    float specialTimer; // Speciális képességek visszatöltéséhez
    Rectangle bounds;   // Pozíció és méret a világtérképen
    int team;
    bool isActive;
} Building;

#define MAX_BUILDINGS 500

typedef struct {
    Building buildings[MAX_BUILDINGS];
    int count;
} BuildingSystem;

// Függvények
void InitBuildingSystem(BuildingSystem* system);
void PlaceBuilding(BuildingSystem* system, BuildingType type, Vector2 position, int team);
void UpdateBuildings(BuildingSystem* system, float deltaTime);
void DrawBuildings(const BuildingSystem* system);

#endif
