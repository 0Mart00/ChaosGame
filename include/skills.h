#ifndef SKILLS_H
#define SKILLS_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    SKILL_DASH, SKILL_DOUBLE_JUMP, SKILL_INVISIBILITY, SKILL_TELEPORT,
    SKILL_LIFE_STEAL, SKILL_CHAIN_LIGHTNING, SKILL_MIND_CONTROL,
    SKILL_GRAVITY_PULL, SKILL_TIME_FREEZE, SKILL_SHIELD_BASH,
    SKILL_POISON_CLOUD, SKILL_SUMMON_MINION, SKILL_BERSERK_RAGE,
    SKILL_FIREBALL, SKILL_ICE_NOVA, SKILL_RESURRECTION,
    SKILL_ARMOR_PIERCE, SKILL_SHOCKWAVE, SKILL_HEALING_AURA,
    SKILL_CLONE_PROJECTION, SKILL_WALL_CLIMB, SKILL_STEALTH_KILL,
    SKILL_METEOR_STRIKE, SKILL_ENERGY_DRAIN, SKILL_EXPLOSIVE_TRAP,
    SKILL_FLIGHT, SKILL_SIZE_ALTERATION, SKILL_PHASE_SHIFT,
    SKILL_VULNERABILITY_HEX, SKILL_REFLECTIVE_BARRIER,
    SKILL_COUNT
} SkillType;

typedef struct {
    SkillType type;
    const char* name;
    float cooldown;      // Újratöltési idő (másodperc)
    float currentCD;     // Aktuális CD állapot
    int manaCost;        // Erőforrás igény
    bool isUnlocked;     // Meg van-e tanulva
} Skill;

// Függvények prototípusai
void InitSkillSystem(void);
void ActivateSkill(SkillType type, int ownerId);
const char* GetSkillDescription(SkillType type);
const char* GetSkillName(int index);


#endif
