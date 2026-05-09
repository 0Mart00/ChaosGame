#include "skills.h"
#include <stdio.h>

// Függvényvázlatok és működési leírások:
static const char* SkillNames[] = {
    "Dash", "Double Jump", "Invisibility", "Teleport", "Life Steal", 
    "Chain Lightning", "Mind Control", "Gravity Pull", "Time Freeze", "Shield Bash",
    "Poison Cloud", "Summon Minion", "Berserk Rage", "Fireball", "Ice Nova", "Resurrection", "Armor Pierce", 
    "Shockwave", "Healing Aura", "Clone Projection", "Wall Climb", "Stealth Kill", "Meteor Strike", "Energy Drain",
    "Explosive Trap", "Flight", "Size Alteration", "Phase Shift", "Vulnerability Hex", "Reflective Barrier"

};


// SKILL_DASH: Hirtelen sebességugrás az egység aktuális irányába.
void Skill_Dash(int ownerId) {}

// SKILL_DOUBLE_JUMP: Lehetővé teszi a második ugrást a levegőben.
void Skill_DoubleJump(int ownerId) {}

// SKILL_INVISIBILITY: Az egység textúrája áttetsző lesz, az ellenfél nem látja.
void Skill_Invisibility(int ownerId) {}

// SKILL_TELEPORT: Az egység azonnal átkerül a kijelölt célpozícióra.
void Skill_Teleport(int ownerId, Vector2 target) {}

// SKILL_LIFE_STEAL: Passzív vagy aktív; minden sebzés egy részét életerővé alakítja.
void Skill_LifeSteal(int ownerId) {}

// SKILL_CHAIN_LIGHTNING: Villám, ami az első célpontról átugrik a közeli ellenfelekre.
void Skill_ChainLightning(int ownerId, int targetId) {}

// SKILL_MIND_CONTROL: Az ellenséges egység ideiglenesen átkerül a mi csapatunkba.
void Skill_MindControl(int ownerId, int targetId) {}

// SKILL_GRAVITY_PULL: Egy pontba rántja a környéken lévő összes egységet.
void Skill_GravityPull(Vector2 point) {}

// SKILL_TIME_FREEZE: Megállítja az időt (Update ciklust) egy adott körzetben.
void Skill_TimeFreeze(Vector2 point, float radius) {}

// SKILL_SHIELD_BASH: Pajzzsal lökés, ami hátralöki és elkábítja (stun) az ellenfelet.
void Skill_ShieldBash(int ownerId, int targetId) {}

// SKILL_POISON_CLOUD: Mérgező gázfelhő, ami folyamatos sebzést (DoT) okoz a területen.
void Skill_PoisonCloud(Vector2 point) {}

// SKILL_SUMMON_MINION: Egy gyengébb egységet idéz az idéző mellé.
void Skill_SummonMinion(int ownerId) {}

// SKILL_BERSERK_RAGE: Megnövelt támadási sebesség és sebzés, de csökkent védelem.
void Skill_BerserkRage(int ownerId) {}

// SKILL_FIREBALL: Robbanó lövedék, ami becsapódáskor területre sebez.
void Skill_Fireball(Vector2 start, Vector2 dir) {}

// SKILL_ICE_NOVA: Fagyasztó hullám az egység körül, ami lassítja/megállítja a környezetet.
void Skill_IceNova(int ownerId) {}

// SKILL_RESURRECTION: Egy elesett szövetséges egység feltámasztása kevés életerővel.
void Skill_Resurrection(Vector2 point) {}

// SKILL_ARMOR_PIERCE: A következő támadás figyelmen kívül hagyja az ellenfél páncélját.
void Skill_ArmorPierce(int ownerId) {}

// SKILL_SHOCKWAVE: A földre ütve minden közeli egységet feldob vagy hátralök.
void Skill_Shockwave(int ownerId) {}

// SKILL_HEALING_AURA: Folyamatos gyógyítás minden szövetségesnek az egység körül.
void Skill_HealingAura(int ownerId) {}

// SKILL_CLONE_PROJECTION: Egy sebzést nem okozó illúziót hoz létre megtévesztésnek.
void Skill_CloneProjection(int ownerId) {}

// SKILL_WALL_CLIMB: Lehetővé teszi az áthaladást az akadályokon (falakon).
void Skill_WallClimb(int ownerId) {}

// SKILL_STEALTH_KILL: Hátulról végrehajtott azonnali kivégzés.
void Skill_StealthKill(int ownerId, int targetId) {}

// SKILL_METEOR_STRIKE: Lassú, de hatalmas erejű becsapódás az égből egy pontra.
void Skill_MeteorStrike(Vector2 target) {}

// SKILL_ENERGY_DRAIN: Ellenség manájának vagy energiájának ellopása.
void Skill_EnergyDrain(int ownerId, int targetId) {}

// SKILL_EXPLOSIVE_TRAP: Láthatatlan csapda a földön, ami rálépéskor robban.
void Skill_ExplosiveTrap(Vector2 point) {}

// SKILL_FLIGHT: Az egység áttér "repülő" módba, kikerülve a földi akadályokat.
void Skill_Flight(int ownerId) {}

// SKILL_SIZE_ALTERATION: Az egység megnő (több HP/Sebzés) vagy összezsugorodik (gyorsabb).
void Skill_SizeAlteration(int ownerId, float scale) {}

// SKILL_PHASE_SHIFT: Rövid ideig tartó sebezhetetlenség, de támadni sem tud közben.
void Skill_PhaseShift(int ownerId) {}

// SKILL_VULNERABILITY_HEX: Átok, ami megnöveli a célponton elszenvedett minden sebzést.
void Skill_VulnerabilityHex(int targetId) {}

// SKILL_REFLECTIVE_BARRIER: Pajzs, ami a kapott sebzés egy részét visszaküldi a támadónak.
void Skill_ReflectiveBarrier(int ownerId) {}




const char* GetSkillName(int index) {
    if (index >= 0 && index < SKILL_COUNT) return SkillNames[index];
    return "Unknown";
}
