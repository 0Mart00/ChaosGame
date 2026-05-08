#ifndef SIMULATION_H
#define SIMULATION_H

#include "modules.h"

#define MAX_ENTITIES 10000

// Entity System - Structure of Arrays (SoA)
typedef struct {
    int count;
    float x[MAX_ENTITIES];
    float y[MAX_ENTITIES];
    float vx[MAX_ENTITIES];
    float vy[MAX_ENTITIES];
    Color color[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} EntitySystem;

// Függvények a szimulációhoz
void InitSimulation(void);
void UpdateSimulation(float deltaTime);
void DrawSimulation(void);

// A modul rajzoló függvénye a menühöz (hogy be tudjuk tölteni a játékot)
void Module_Simulation_Draw(GameState *currentState);

#endif
