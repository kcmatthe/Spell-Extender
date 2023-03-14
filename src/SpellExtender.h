#pragma once

#include "Serialization.h"

namespace SpellExtenderNamespace {
	//Spell Crafting
	static void copySpell(RE::StaticFunctionTag*, RE::SpellItem* newSpell, RE::SpellItem* sourceSpell);
	bool combineSpells(RE::StaticFunctionTag*, RE::SpellItem* akBase, std::vector<RE::SpellItem*> akSpells, RE::BSFixedString akName);
	//Spell Functions
	static void setChargeTime(RE::StaticFunctionTag*, RE::SpellItem* spell, float time);
	static void setChargeTimeAll(RE::StaticFunctionTag*, RE::SpellItem* spell, float time);
	static void setSpellCost(RE::StaticFunctionTag*, RE::SpellItem* spell, int cost);
	static void setSpellFlag(RE::StaticFunctionTag*, RE::SpellItem* spell, int flag, int bit);
	bool IsManualCalc(RE::StaticFunctionTag*, RE::SpellItem* spell);
	bool IgnoresResistance(RE::StaticFunctionTag* base, RE::SpellItem* spell);
	bool IgnoresLOS(RE::StaticFunctionTag*, RE::SpellItem* spell);
	bool IsNoAbsorb(RE::StaticFunctionTag*, RE::SpellItem* spell);
	bool IsNoDualCastMod(RE::StaticFunctionTag*, RE::SpellItem* spell);
	bool IsPCStartSpell(RE::StaticFunctionTag*, RE::SpellItem* spell);
	//Magic Effect Functions
	static void setNthEffectDelivery(RE::StaticFunctionTag* base, RE::SpellItem* spell, int delivery, int index);
	static void setNthEffectArchetype(RE::StaticFunctionTag* base, RE::SpellItem* spell, int archetype, int index);
	static void setNthEffectPrimaryAV(RE::StaticFunctionTag* base, RE::SpellItem* spell, int av, int index);
	static void setNthEffectSecondaryAV(RE::StaticFunctionTag* base, RE::SpellItem* spell, int av, int index);
	static void setNthEffectSecondaryAVWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, float weight, int index);
	static void setNthEffectTaperValues(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapDur, float tapWeight, float tapCurve, int index);
	static void setNthEffectTaperDuration(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapDur, int index);
	static void setNthEffectTaperWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapWeight, int index);
	static void setNthEffectTaperCurve(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapCurve, int index);
	static void setNthEffectCastingSoundLevel(RE::StaticFunctionTag* base, RE::SpellItem* spell, int level, int index);
	static void setNthEffectDualCastScale(RE::StaticFunctionTag* base, RE::SpellItem* spell, float scale, int index);
	//Getters
	float getNthEffectSecondaryAVWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	float getNthEffectTaperDuration(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	float getNthEffectTaperWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	float getNthEffectTaperCurve(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	int getNthEffectCastingSoundLevel(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	float getNthEffectDualCastScale(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index);
	
	bool RegisterFuncs(RE::BSScript::IVirtualMachine* registry);

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
}
