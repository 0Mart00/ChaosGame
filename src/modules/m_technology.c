#include "technology.h"

static const char* TechNames[] = {
"Nanobot Repair", "Fusion Reactor", "Neural Link",
"Plasma Coating", "Antimatter Engine", "Quantum Computing",
"Cyborg Augmentation", "Orbital Strike", "Shield Overload",
"Bio-Luminescence", "Laser Guidance", "Cryo-Storage",
"Jetpack Propulsion", "Smart Ammo", "Holographic Decoy",
"Genetic Engineering", "Subterranean Drill", "Gamma Ray Burst",
"Exoskeleton Suit", "Atmosphere Processor", "Warp Drive",
"Sonic Dampener", "Magnetic Railgun", "AI Autopilot",
"Thermal Vision", "Energy Harvesting", "Stealth Plating",
"Telepathic Uplink", "Matter Synthesis", "Force Field Dome"
};


// TECH_NANOBOT_REPAIR: Az egységek és épületek passzív, lassú öngyógyítása harcon kívül.
void Tech_NanobotRepair() {}

// TECH_FUSION_REACTOR: Drasztikusan megnöveli az alapvető energia-termelést.
void Tech_FusionReactor() {}

// TECH_NEURAL_LINK: Gyorsabb egység-reakcióidő és megnövelt látótávolság.
void Tech_NeuralLink() {}

// TECH_PLASMA_COATING: Plusz páncélzat a lövedékek ellen, elnyeli a sebzés egy részét.
void Tech_PlasmaCoating() {}

// TECH_ANTIMATTER_ENGINE: Minden jármű és űrhajó végsebességének növelése.
void Tech_AntimatterEngine() {}

// TECH_QUANTUM_COMPUTING: Felgyorsítja a további technológiák kutatási idejét.
void Tech_QuantumComputing() {}

// TECH_CYBORG_AUGMENTATION: Gyalogsági egységek HP-jának és erejének növelése.
void Tech_CyborgAugmentation() {}

// TECH_ORBITAL_STRIKE: Új aktív képesség: pusztító csapás az égből a térkép bármely pontjára.
void Tech_OrbitalStrike() {}

// TECH_SHIELD_OVERLOAD: A pajzsok felrobbanhatnak megsemmisüléskor, sebezve a támadót.
void Tech_ShieldOverload() {}

// TECH_BIO_LUMINESCENCE: Az egységek fényt bocsátanak ki, eloszlatva a "háború ködét".
void Tech_BioLuminescence() {}

// TECH_LASER_GUIDANCE: Megnövelt pontosság és hatótáv távolsági fegyvereknél.
void Tech_LaserGuidance() {}

// TECH_CRYO_STORAGE: Nyersanyag-tárolási limit növelése és veszteségek csökkentése.
void Tech_CryoStorage() {}

// TECH_JETPACK_PROPULSION: Gyalogság számára lehetővé teszi rövid távolságok átrepülését.
void Tech_JetpackPropulsion() {}

// TECH_SMART_AMMO: A lövedékek kis mértékben követik a mozgó célpontokat.
void Tech_SmartAmmo() {}

// TECH_HOLOGRAPHIC_DECOY: Épületek képesek hamis képmást vetíteni magukról.
void Tech_HolographicDecoy() {}

// TECH_GENETIC_ENGINEERING: Biológiai egységek gyorsabb kiképzése.
void Tech_GeneticEngineering() {}

// TECH_SUBTERRANEAN_DRILL: Föld alatti nyersanyagforrások hatékonyabb kinyerése.
void Tech_SubterraneanDrill() {}

// TECH_GAMMA_RAY_BURST: Sugárfegyverek átütő erejének és területsebzésének növelése.
void Tech_GammaRayBurst() {}

// TECH_EXOSKELETON_SUIT: Gyalogsági egységek teherbírásának és sebességének növelése.
void Tech_ExoskeletonSuit() {}

// TECH_ATMOSPHERE_PROCESSOR: Lassítja a környezeti ártalmakat és javítja a látási viszonyokat.
void Tech_AtmosphereProcessor() {}

// TECH_WARP_DRIVE: Lehetővé teszi az azonnali ugrást távoli szektorok között.
void Tech_WarpDrive() {}

// TECH_SONIC_DAMPENER: Az egységek mozgása nem jelenik meg az ellenséges radaron.
void Tech_SonicDampener() {}

// TECH_MAGNETIC_RAILGUN: Fizikai alapú fegyverek lövedéksebességének drasztikus növelése.
void Tech_MagneticRailgun() {}

// TECH_AI_AUTOPILOT: Az egységek okosabban vonulnak fedezékbe, ha nem kapnak parancsot.
void Tech_AIAutopilot() {}

// TECH_THERMAL_VISION: Láthatóvá teszi a Stealth (lopakodó) egységeket.
void Tech_ThermalVision() {}

// TECH_ENERGY_HARVESTING: A kilőtt lövedékek energiájának egy részét visszanyeri a hálózat.
void Tech_EnergyHarvesting() {}

// TECH_STEALTH_PLATING: Passzív álcázás járművek számára álló helyzetben.
void Tech_StealthPlating() {}

// TECH_TELEPATHIC_UPLINK: Globális egységirányítás: nincs szükség parancsnoki hatótávra.
void Tech_TelepathicUplink() {}

// TECH_MATTER_SYNTHESIS: Energiából képes alapvető építőanyagokat (fém, kristály) gyártani.
void Tech_MatterSynthesis() {}

// TECH_FORCE_FIELD_DOME: Fix védelmi épület, amely ernyőt von a bázis fölé.
void Tech_ForceFieldDome() {}


const char* GetTechName(int index) {
    if (index >= 0 && index < TECH_COUNT) return TechNames[index];
    return "Unknown";
}
