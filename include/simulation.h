#ifndef SIMULATION_H
#define SIMULATION_H

#include "raylib.h"
#include "modules.h"
#include <stdbool.h>

#define MAX_ENTITIES 10000

// Entity System - Structure of Arrays (SoA)
typedef struct  EntitySystem{
    float x[MAX_ENTITIES];
    float y[MAX_ENTITIES];
    float vx[MAX_ENTITIES];
    float vy[MAX_ENTITIES];
    int team[MAX_ENTITIES]; // 0: Player, 1: Enemy, stb.
    Color color[MAX_ENTITIES]; // <--- EZ HIÁNYZOTT
    bool active[MAX_ENTITIES];
    int count;
} EntitySystem;

// Függvények a szimulációhoz
void InitSimulation(void);
void UpdateSimulation(float deltaTime);
void DrawSimulation(void);

// A modul rajzoló függvénye a menühöz (hogy be tudjuk tölteni a játékot)
void Module_Simulation_Draw(GameState *currentState);

#endif
