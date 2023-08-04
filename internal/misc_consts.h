#pragma once

enum ActorValueCode : UInt32
{
	/*00*/kAVCode_Aggression,
	/*01*/kAVCode_Confidence,
	/*02*/kAVCode_Energy,
	/*03*/kAVCode_Responsibility,
	/*04*/kAVCode_Mood,
	/*05*/kAVCode_Strength,
	/*06*/kAVCode_Perception,
	/*07*/kAVCode_Endurance,
	/*08*/kAVCode_Charisma,
	/*09*/kAVCode_Intelligence,
	/*0A*/kAVCode_Agility,
	/*0B*/kAVCode_Luck,
	/*0C*/kAVCode_ActionPoints,
	/*0D*/kAVCode_CarryWeight,
	/*0E*/kAVCode_CritChance,
	/*0F*/kAVCode_HealRate,
	/*10*/kAVCode_Health,
	/*11*/kAVCode_MeleeDamage,
	/*12*/kAVCode_DamageResist,
	/*13*/kAVCode_PoisonResist,
	/*14*/kAVCode_RadResist,
	/*15*/kAVCode_SpeedMult,
	/*16*/kAVCode_Fatigue,
	/*17*/kAVCode_Karma,
	/*18*/kAVCode_XP,
	/*19*/kAVCode_PerceptionCondition,
	/*1A*/kAVCode_EnduranceCondition,
	/*1B*/kAVCode_LeftAttackCondition,
	/*1C*/kAVCode_RightAttackCondition,
	/*1D*/kAVCode_LeftMobilityCondition,
	/*1E*/kAVCode_RightMobilityCondition,
	/*1F*/kAVCode_BrainCondition,
	/*20*/kAVCode_Barter,
	/*21*/kAVCode_BigGuns,
	/*22*/kAVCode_EnergyWeapons,
	/*23*/kAVCode_Explosives,
	/*24*/kAVCode_Lockpick,
	/*25*/kAVCode_Medicine,
	/*26*/kAVCode_MeleeWeapons,
	/*27*/kAVCode_Repair,
	/*28*/kAVCode_Science,
	/*29*/kAVCode_Guns,
	/*2A*/kAVCode_Sneak,
	/*2B*/kAVCode_Speech,
	/*2C*/kAVCode_Survival,
	/*2D*/kAVCode_Unarmed,
	/*2E*/kAVCode_InventoryWeight,
	/*2F*/kAVCode_Paralysis,
	/*30*/kAVCode_Invisibility,
	/*31*/kAVCode_Chameleon,
	/*32*/kAVCode_NightEye,
	/*33*/kAVCode_Turbo,
	/*34*/kAVCode_FireResist,
	/*35*/kAVCode_WaterBreathing,
	/*36*/kAVCode_RadiationRads,
	/*37*/kAVCode_BloodyMess,
	/*38*/kAVCode_UnarmedDamage,
	/*39*/kAVCode_Assistance,
	/*3A*/kAVCode_ElectricResist,
	/*3B*/kAVCode_FrostResist,
	/*3C*/kAVCode_EnergyResist,
	/*3D*/kAVCode_EmpResist,
	/*3E*/kAVCode_Variable01,
	/*3F*/kAVCode_Variable02,
	/*40*/kAVCode_Variable03,
	/*41*/kAVCode_Variable04,
	/*42*/kAVCode_Variable05,
	/*43*/kAVCode_Variable06,
	/*44*/kAVCode_Variable07,
	/*45*/kAVCode_Variable08,
	/*46*/kAVCode_Variable09,
	/*47*/kAVCode_Variable10,
	/*48*/kAVCode_IgnoreCrippledLimbs,
	/*49*/kAVCode_Dehydration,
	/*4A*/kAVCode_Hunger,
	/*4B*/kAVCode_SleepDeprivation,
	/*4C*/kAVCode_DamageThreshold,
	kAVCode_Max
};

enum MiscStatCode : UInt32
{
	/*00*/kMiscStat_QuestsCompleted,
	/*01*/kMiscStat_LocationsDiscovered,
	/*02*/kMiscStat_PeopleKilled,
	/*03*/kMiscStat_CreaturesKilled,
	/*04*/kMiscStat_LocksPicked,
	/*05*/kMiscStat_ComputersHacked,
	/*06*/kMiscStat_StimpaksTaken,
	/*07*/kMiscStat_RadXTaken,
	/*08*/kMiscStat_RadAwayTaken,
	/*09*/kMiscStat_ChemsTaken,
	/*0A*/kMiscStat_TimesAddicted,
	/*0B*/kMiscStat_MinesDisarmed,
	/*0C*/kMiscStat_SpeechSuccesses,
	/*0D*/kMiscStat_PocketsPicked,
	/*0E*/kMiscStat_PantsExploded,
	/*0F*/kMiscStat_BooksRead,
	/*10*/kMiscStat_HealthFromStimpaks,
	/*11*/kMiscStat_WeaponsCreated,
	/*12*/kMiscStat_HealthFromFood,
	/*13*/kMiscStat_WaterConsumed,
	/*14*/kMiscStat_SandmanKills,
	/*15*/kMiscStat_ParalyzingPunches,
	/*16*/kMiscStat_RobotsDisabled,
	/*17*/kMiscStat_TimesSlept,
	/*18*/kMiscStat_CorpsesEaten,
	/*19*/kMiscStat_MysteriousStrangerVisits,
	/*1A*/kMiscStat_DoctorBagsUsed,
	/*1B*/kMiscStat_ChallengesCompleted,
	/*1C*/kMiscStat_MissFortunateOccurrences,
	/*1D*/kMiscStat_Disintegrations,
	/*1E*/kMiscStat_HaveLimbsCrippled,
	/*1F*/kMiscStat_SpeechFailures,
	/*20*/kMiscStat_ItemsCrafted,
	/*21*/kMiscStat_WeaponModifications,
	/*22*/kMiscStat_ItemsRepaired,
	/*23*/kMiscStat_TotalThingsKilled,
	/*24*/kMiscStat_DismemberedLimbs,
	/*25*/kMiscStat_CaravanGamesWon,
	/*26*/kMiscStat_CaravanGamesLost,
	/*27*/kMiscStat_BarterAmountTraded,
	/*28*/kMiscStat_RouletteGamesPlayed,
	/*29*/kMiscStat_BlackjackGamesPlayed,
	/*2A*/kMiscStat_SlotsGamesPlayed,
	kMiscStat_Max
};

enum PerkEntryPointID : UInt32
{
	/*00*/kPerkEntry_CalculateWeaponDamage,
	/*01*/kPerkEntry_CalculateMyCriticalHitChance,
	/*02*/kPerkEntry_CalculateMyCriticalHitDamage,
	/*03*/kPerkEntry_CalculateWeaponAttackAPCost,
	/*04*/kPerkEntry_CalculateMineExplodeChance,
	/*05*/kPerkEntry_AdjustRangePenalty,
	/*06*/kPerkEntry_AdjustLimbDamage,
	/*07*/kPerkEntry_CalculateWeaponRange,
	/*08*/kPerkEntry_CalculateToHitChance,
	/*09*/kPerkEntry_AdjustExperiencePoints,
	/*0A*/kPerkEntry_AdjustGainedSkillPoints,
	/*0B*/kPerkEntry_AdjustBookSkillPoints,
	/*0C*/kPerkEntry_ModifyRecoveredHealth,
	/*0D*/kPerkEntry_CalculateInventoryAPCost,
	/*0E*/kPerkEntry_GetDisposition,
	/*0F*/kPerkEntry_GetShouldAttack,
	/*10*/kPerkEntry_GetShouldAssist,
	/*11*/kPerkEntry_CalculateBuyPrice,
	/*12*/kPerkEntry_GetBadKarma,
	/*13*/kPerkEntry_GetGoodKarma,
	/*14*/kPerkEntry_IgnoreLockedTerminal,
	/*15*/kPerkEntry_AddLeveledListOnDeath,
	/*16*/kPerkEntry_GetMaxCarryWeight,
	/*17*/kPerkEntry_ModifyAddictionChance,
	/*18*/kPerkEntry_ModifyAddictionDuration,
	/*19*/kPerkEntry_ModifyPositiveChemDuration,
	/*1A*/kPerkEntry_AdjustDrinkingRadiation,
	/*1B*/kPerkEntry_Activate,
	/*1C*/kPerkEntry_MysteriousStranger,
	/*1D*/kPerkEntry_HasParalyzingPalm,
	/*1E*/kPerkEntry_HackingScienceBonus,
	/*1F*/kPerkEntry_IgnoreRunningDuringDetection,
	/*20*/kPerkEntry_IgnoreBrokenLock,
	/*21*/kPerkEntry_HasConcentratedFire,
	/*22*/kPerkEntry_CalculateGunSpread,
	/*23*/kPerkEntry_PlayerKillAPReward,
	/*24*/kPerkEntry_ModifyEnemyCriticalHitChance,
	/*25*/kPerkEntry_ReloadSpeed,
	/*26*/kPerkEntry_EquipSpeed,
	/*27*/kPerkEntry_ActionPointRegen,
	/*28*/kPerkEntry_ActionPointCost,
	/*29*/kPerkEntry_MissFortune,
	/*2A*/kPerkEntry_ModifyRunSpeed,
	/*2B*/kPerkEntry_ModifyAttackSpeed,
	/*2C*/kPerkEntry_ModifyRadiationConsumed,
	/*2D*/kPerkEntry_HasPipHacker,
	/*2E*/kPerkEntry_HasMeltdown,
	/*2F*/kPerkEntry_SeeEnemyHealth,
	/*30*/kPerkEntry_HasJuryRigging,
	/*31*/kPerkEntry_ModifyThreatRange,
	/*32*/kPerkEntry_ModifyThreat,
	/*33*/kPerkEntry_HasFastTravelAlways,
	/*34*/kPerkEntry_KnockdownChance,
	/*35*/kPerkEntry_ModifyWeaponStrengthReq,
	/*36*/kPerkEntry_ModifyAimingMoveSpeed,
	/*37*/kPerkEntry_ModifyLightItems,
	/*38*/kPerkEntry_ModifyDamageThresholdDefender,
	/*39*/kPerkEntry_ModifyChanceforAmmoItem,
	/*3A*/kPerkEntry_ModifyDamageThresholdAttacker,
	/*3B*/kPerkEntry_ModifyThrowingVelocity,
	/*3C*/kPerkEntry_ChanceForItemOnFire,
	/*3D*/kPerkEntry_HasUnarmedForwardPowerAttack,
	/*3E*/kPerkEntry_HasUnarmedBackPowerAttack,
	/*3F*/kPerkEntry_HasUnarmedCrouchedPowerAttack,
	/*40*/kPerkEntry_HasUnarmedCounterAttack,
	/*41*/kPerkEntry_HasUnarmedLeftPowerAttack,
	/*42*/kPerkEntry_HasUnarmedRightPowerAttack,
	/*43*/kPerkEntry_VATSHelperChance,
	/*44*/kPerkEntry_ModifyItemDamage,
	/*45*/kPerkEntry_HasImprovedDetection,
	/*46*/kPerkEntry_HasImprovedSpotting,
	/*47*/kPerkEntry_HasImprovedItemDetection,
	/*48*/kPerkEntry_AdjustExplosionRadius,
	/*49*/kPerkEntry_AdjustHeavyWeaponWeight,
	kPerkEntry_Max
};

enum SpecialInputCode
{
	kInputCode_Backspace =	0x80000000,
	kInputCode_ArrowLeft =	0x80000001,
	kInputCode_ArrowRight =	0x80000002,
	kInputCode_ArrowUp =	0x80000003,
	kInputCode_ArrowDown =	0x80000004,
	kInputCode_Home =		0x80000005,
	kInputCode_End =		0x80000006,
	kInputCode_Delete =		0x80000007,
	kInputCode_Enter =		0x80000008,
	kInputCode_PageUp =		0x80000009,
	kInputCode_PageDown =	0x8000000A
};

enum MaterialType
{
	kMaterial_Stone,
	kMaterial_Dirt,
	kMaterial_Grass,
	kMaterial_Glass,
	kMaterial_Metal,
	kMaterial_Wood,
	kMaterial_Organic,
	kMaterial_Cloth,
	kMaterial_Water,
	kMaterial_HollowMetal,
	kMaterial_OrganicBug,
	kMaterial_OrganicGlow
};

#define ADDR_AddExtraData		0x40FF60
#define ADDR_RemoveExtraType	0x410140
#define ADDR_LoadModel			0x447080
#define ADDR_ApplyAmmoEffects	0x59A030
#define ADDR_MoveToMarker		0x5CCB20
#define ADDR_ApplyPerkModifiers	0x5E58F0
#define ADDR_ReturnThis			0x6815C0
#define ADDR_PurgeTerminalModel	0x7FFE00
#define ADDR_EquipItem			0x88C650
#define ADDR_UnequipItem		0x88C790
#define ADDR_ReturnTrue			0x8D0360
#define ADDR_TileGetFloat		0xA011B0
#define ADDR_TileSetFloat		0xA012D0
#define ADDR_TileSetString		0xA01350
#define ADDR_InitFontInfo		0xA12020
#define ADDR_GetRandomInt		0xAA5230
#define ADDR_ReturnThis2		0xE68810

#define EXTRA_DATA_CS			0x11C3920
#define SCENE_LIGHTS_CS			0x11F9EA0

#define IS_REFERENCE(form) ((*(UInt32**)form)[0x3C] == ADDR_ReturnTrue)
#define NOT_REFERENCE(form) ((*(UInt32**)form)[0x3C] != ADDR_ReturnTrue)
#define IS_ACTOR(form) ((*(UInt32**)form)[0x40] == ADDR_ReturnTrue)
#define NOT_ACTOR(form) ((*(UInt32**)form)[0x40] != ADDR_ReturnTrue)
#define IS_NODE(object) ((*(UInt32**)object)[3] == ADDR_ReturnThis)
#define IS_GEOMETRY(object) ((*(UInt32**)object)[6] == ADDR_ReturnThis2)

extern const bool kInventoryType[];
extern const UInt8 kMaterialConvert[];
extern const char kDaysPerMonth[], kMenuIDJumpTable[];
extern bool s_NPCWeaponMods, s_NPCPerks;