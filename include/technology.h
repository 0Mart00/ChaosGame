#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    TECH_NANOBOT_REPAIR, TECH_FUSION_REACTOR, TECH_NEURAL_LINK, 
    TECH_PLASMA_COATING, TECH_ANTIMATTER_ENGINE, TECH_QUANTUM_COMPUTING,
    TECH_CYBORG_AUGMENTATION, TECH_ORBITAL_STRIKE, TECH_SHIELD_OVERLOAD,
    TECH_BIO_LUMINESCENCE, TECH_LASER_GUIDANCE, TECH_CRYO_STORAGE,
    TECH_JETPACK_PROPULSION, TECH_SMART_AMMO, TECH_HOLOGRAPHIC_DECOY,
    TECH_GENETIC_ENGINEERING, TECH_SUBTERRANEAN_DRILL, TECH_GAMMA_RAY_BURST,
    TECH_EXOSKELETON_SUIT, TECH_ATMOSPHERE_PROCESSOR, TECH_WARP_DRIVE,
    TECH_SONIC_DAMPENER, TECH_MAGNETIC_RAILGUN, TECH_AI_AUTOPILOT,
    TECH_THERMAL_VISION, TECH_ENERGY_HARVESTING, TECH_STEALTH_PLATING,
    TECH_TELEPATHIC_UPLINK, TECH_MATTER_SYNTHESIS, TECH_FORCE_FIELD_DOME,
    TECH_COUNT
} TechType;

typedef struct {
    TechType type;
    const char* name;
    int researchCost;    // Kutatási pont vagy nyersanyag igény
    bool isResearched;   // Ki van-e fejlesztve
    int requiredLevel;   // Szükséges főhadiszállás szint
} Technology;

void InitTechSystem(void);
void ResearchTech(TechType type);
bool IsTechActive(TechType type);
const char* GetTechName(int index);



#endif
