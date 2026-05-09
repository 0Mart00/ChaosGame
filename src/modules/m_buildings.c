#include "buildings.h"
#include <string.h>

static const char* BuildingNames[] = {
    "Bar", "Temple", "Police Station", "Town Hall", 
    "Terraformer", "Mirage Tower", "Gravity Well", "Hacker Den",
    "Scrap Yard", "Trading Post", "Weather Lab", "Alchemy Lab"
};

void InitBuildingSystem(BuildingSystem* system) {
    system->count = 0;
    for(int i = 0; i < MAX_BUILDINGS; i++) {
        system->buildings[i].isActive = false;
    }
}

void PlaceBuilding(BuildingSystem* system, BuildingType type, Vector2 position, int team) {
    if (system->count >= MAX_BUILDINGS) return;

    Building* b = &system->buildings[system->count];
    b->type = type;
    b->name = BuildingNames[type];
    b->maxHealth = 1000;
    b->health = b->maxHealth;
    b->team = team;
    b->isActive = true;
    b->specialTimer = 0;
    // Az épületek alapmérete 64x64 pixel (2x2 tile)
    b->bounds = (Rectangle){ position.x, position.y, 64, 64 };

    system->count++;
}

void UpdateBuildings(BuildingSystem* system, float deltaTime) {
    for (int i = 0; i < system->count; i++) {
        Building* b = &system->buildings[i];
        if (!b->isActive) continue;

        b->specialTimer += deltaTime;

        // Egyedi logikák (Példák)
        switch (b->type) {
            case BLD_GRAVITY_WELL:
                // Itt lehetne egy körkörös ütközésvizsgálat lövedékekre
                break;
            case BLD_HACKER_DEN:
                if (b->specialTimer > 10.0f) { /* Hack akció */ }
                break;
            case BLD_WEATHER_LAB:
                // Köd generálása a pozíció körül
                break;
            default: break;
        }
    }
}

void DrawBuildings(const BuildingSystem* system) {
    for (int i = 0; i < system->count; i++) {
        const Building* b = &system->buildings[i];
        if (!b->isActive) continue;

        // Csapatfüggő szín
        Color teamColor = (b->team == 0) ? BLUE : RED;
        
        // Épület teste
        DrawRectangleRec(b->bounds, teamColor);
        DrawRectangleLinesEx(b->bounds, 2, WHITE);
        
        // Életerő csík
        float healthPct = (float)b->health / b->maxHealth;
        DrawRectangle(b->bounds.x, b->bounds.y - 10, b->bounds.width, 5, BLACK);
        DrawRectangle(b->bounds.x, b->bounds.y - 10, b->bounds.width * healthPct, 5, GREEN);

        // Név megjelenítése
        DrawText(b->name, b->bounds.x, b->bounds.y + b->bounds.height + 2, 10, WHITE);
    }
}

const char* GetBuildingName(BuildingType type) {
    if (type >= 0 && type < BLD_COUNT) {
        return BuildingNames[type];
    }
    return "Unknown";
}

void DestroyBuilding(BuildingSystem* system, int index) {
    if (index >= 0 && index < system->count) {
        system->buildings[index].isActive = false;
    }
}

void UpgradeBuilding(BuildingSystem* system, int index) {
    if (index >= 0 && index < system->count) {
        Building* b = &system->buildings[index];
        b->maxHealth += 500;
        b->health = b->maxHealth;
        // Itt adhatsz hozzá szintlépést: b->level++;
    }
}




