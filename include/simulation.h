#ifndef SIMULATION_H
#define SIMULATION_H

#include "raylib.h"
#include "modules.h"
#include <stdbool.h>

#define MAX_ENTITIES 10000

// Entity System - Structure of Arrays (SoA)
typedef struct EntitySystem {
    float x[MAX_ENTITIES], y[MAX_ENTITIES];
    float vx[MAX_ENTITIES], vy[MAX_ENTITIES];
    float targetX[MAX_ENTITIES], targetY[MAX_ENTITIES]; // Célpontok
    bool isSelected[MAX_ENTITIES]; // Kijelölési állapot
    bool hasTarget[MAX_ENTITIES];  // Van-e aktív parancsa
    int team[MAX_ENTITIES];
    Color color[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
    int count;
} EntitySystem;

// Függvények a szimulációhoz
void InitSimulation(void);
void UpdateSimulation(float deltaTime);
void DrawSimulation(void);


void SetPlacementMode(int buildingType); // <--- TEDD BE include
// A modul rajzoló függvénye a menühöz (hogy be tudjuk tölteni a játékot)
void Module_Simulation_Draw(GameState *currentState);

#endif
