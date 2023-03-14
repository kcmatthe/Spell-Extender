#include "SpellExtender.h"

namespace SpellExtenderNamespace {
    
	std::set<RE::SpellItem*> modifiedSpells;

	//Spell crafting?
	//copyEffect() ??
	
	//this is a helper function for copy spell
	bool _copySpell(RE::SpellItem* newSpell, RE::SpellItem* sourceSpell) {
		
		newSpell->equipSlot = sourceSpell->equipSlot;
		newSpell->menuDispObject = sourceSpell->menuDispObject;
		newSpell->descriptionText = sourceSpell->descriptionText;
		newSpell->data = sourceSpell->data;
		newSpell->effects = sourceSpell->effects;
		newSpell->formFlags = sourceSpell->formFlags;
		newSpell->fullName = sourceSpell->fullName;
		newSpell->keywords = sourceSpell->keywords;
		newSpell->hostileCount = sourceSpell->hostileCount;
		//newSpell->boundData = sourceSpell->boundData;
		//newSpell->pad14 = sourceSpell->pad14;
		//newSpell->fileOffset = sourceSpell->fileOffset;
		//newSpell->sourceFiles = sourceSpell->sourceFiles;
		//newSpell->preloadCount = sourceSpell->preloadCount;
		//newSpell->pad1B = sourceSpell->pad1B;
		//newSpell->pad1C = sourceSpell->pad1C;
		//newSpell->pad2C = sourceSpell->pad2C;
		//newSpell->pad74 = sourceSpell->pad74;
		//newSpell->pad84 = sourceSpell->pad84;
		newSpell->formType = sourceSpell->formType;
		newSpell->data.castingType = sourceSpell->data.castingType;
		newSpell->data.costOverride = sourceSpell->data.costOverride;
		newSpell->data.range = sourceSpell->data.range;
		newSpell->data.chargeTime = sourceSpell->data.chargeTime;
		newSpell->data.castingPerk = sourceSpell->data.castingPerk;
		newSpell->data.flags = sourceSpell->data.flags;
		newSpell->data.delivery = sourceSpell->data.delivery;
		newSpell->data.spellType = sourceSpell->data.spellType;
		newSpell->data.castDuration = sourceSpell->data.castDuration;


		return true;
	}

	static void copySpell(RE::StaticFunctionTag*, RE::SpellItem* newSpell, RE::SpellItem* sourceSpell)
	{
		logger::info("Source spell: {}", sourceSpell->fullName.c_str());
		logger::info("Copied into new spell: {}", newSpell->fullName.c_str());

		_copySpell(newSpell, sourceSpell);

		modifiedSpells.insert(newSpell);

	}
	bool combineSpells(RE::StaticFunctionTag*, RE::SpellItem* akBase, std::vector<RE::SpellItem*> akSpells, RE::BSFixedString akName)
	{
		//RE::SpellItem* newSpell = NULL; 
		if (!akBase || akSpells.empty() || !akSpells[0]) {
			logger::info("NO SPELLS TO COMBINE");
			return false;
		}
		//newSpell = akSpells[0];
		_copySpell(akBase, akSpells[0]);
		akBase->fullName = akName;
		for (auto& spell : akSpells) {
			if (!spell) {
				break;
			}
			for (auto& effect : spell->effects) {
				akBase->effects.emplace_back(effect);
				logger::info("added effect {}", effect->baseEffect->fullName.c_str());
			}
		}

		modifiedSpells.insert(akBase);
		return true;
	}
	//Functions targeting Spells

	static void setChargeTime(RE::StaticFunctionTag*, RE::SpellItem* spell, float time) {
        spell->data.chargeTime = time;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("charge time set to: {}", time);

		modifiedSpells.insert(spell);
    }
	static void setChargeTimeAll(RE::StaticFunctionTag*, RE::SpellItem* spell, float time)
	{
		spell->data.chargeTime = time;
		for (const auto& effect : spell->effects) {
			if (const auto baseEffect = effect ? effect->baseEffect : nullptr; baseEffect) {
				baseEffect->data.spellmakingChargeTime = time;
			}
		}

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Charge time of spell and effects set to: {}", time);
		modifiedSpells.insert(spell);
	}
	static void setSpellCost(RE::StaticFunctionTag* base, RE::SpellItem* spell, int cost) {
		spell->data.costOverride = cost;
		
		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Spell cost set to: {}", cost);
		modifiedSpells.insert(spell);
	}

	//static void getSpellCost(RE::StaticFunctionTag* base, RE::SpellItem* spell) {
	//	return spell->data.costOverride;
	//}
	static void setSpellFlag(RE::StaticFunctionTag*, RE::SpellItem* spell, int flag, int bit) {
		auto flagsVal = spell->RE::SpellItem::GetData()->flags;
		spell->RE::SpellItem::GetData()->flags = (flagsVal & ~(1 << flag)) | (bit << flag);
		
		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("The flag being set is: {}", flag);
		logger::info("Flag set to: {}", bit);
		modifiedSpells.insert(spell);
	}
	bool IsManualCalc(RE::StaticFunctionTag*, RE::SpellItem* spell) {
		return (spell->RE::SpellItem::GetData()->flags >> 0) & 1;
	}
	bool IgnoresResistance(RE::StaticFunctionTag*, RE::SpellItem* spell)
	{
		return (spell->RE::SpellItem::GetData()->flags >> 19) & 1;
	}
	bool IgnoresLOS(RE::StaticFunctionTag*, RE::SpellItem* spell)
	{
		return (spell->RE::SpellItem::GetData()->flags >> 20) & 1;
	}
	bool IsNoAbsorb(RE::StaticFunctionTag*, RE::SpellItem* spell)
	{
		return (spell->RE::SpellItem::GetData()->flags >> 21) & 1;
		;
	}
	bool IsNoDualCastMod(RE::StaticFunctionTag*, RE::SpellItem* spell)
	{
		return (spell->RE::SpellItem::GetData()->flags >> 23) & 1;
		
	}
	bool IsPCStartSpell(RE::StaticFunctionTag*, RE::SpellItem* spell)
	{
		return (spell->RE::SpellItem::GetData()->flags >> 17) & 1;
	}

	//Functions targeting Magic Effects
		//Setters

	static void setNthEffectDelivery(RE::StaticFunctionTag* base, RE::SpellItem* spell, int delivery, int index)
	{
		const auto type = static_cast<RE::MagicSystem::Delivery>(delivery);
		spell->effects[index]->baseEffect->data.delivery = type;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect delivery set to: {}", delivery);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectArchetype(RE::StaticFunctionTag* base, RE::SpellItem* spell, int archetype, int index) {
		const auto type = static_cast<RE::EffectSetting::Archetype>(archetype);
		spell->effects[index]->baseEffect->data.archetype = type;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect archetype set to: {}", archetype);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectPrimaryAV(RE::StaticFunctionTag* base, RE::SpellItem* spell, int av, int index) {
		const auto type = static_cast<RE::ActorValue>(av);
		spell->effects[index]->baseEffect->data.primaryAV = type;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect primary av set to: {}", av);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectSecondaryAV(RE::StaticFunctionTag* base, RE::SpellItem* spell, int av, int index)
	{
		const auto type = static_cast<RE::ActorValue>(av);
		spell->effects[index]->baseEffect->data.secondaryAV = type;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect secondary av set to: {}", av);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectSecondaryAVWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, float weight, int index)
	{
		spell->effects[index]->baseEffect->data.secondAVWeight = weight;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect secondary av weight set to: {}", weight);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectTaperValues(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapDur, float tapWeight, float tapCurve, int index) {
		spell->effects[index]->baseEffect->data.taperDuration = tapDur;
		spell->effects[index]->baseEffect->data.taperWeight = tapWeight;
		spell->effects[index]->baseEffect->data.taperCurve = tapCurve;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect taperCurve set to: {}", tapCurve);
		logger::info("Effect taper duration set to: {}", tapDur);
		logger::info("Effect taper weight set to: {}", tapWeight);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectTaperDuration(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapDur, int index) {
		spell->effects[index]->baseEffect->data.taperDuration = tapDur;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect taper duration set to: {}", tapDur);
		modifiedSpells.insert(spell);

	}
	static void setNthEffectTaperWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapWeight, int index)
	{
		spell->effects[index]->baseEffect->data.taperWeight = tapWeight;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect taper weight set to: {}", tapWeight);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectTaperCurve(RE::StaticFunctionTag* base, RE::SpellItem* spell, float tapCurve, int index)
	{
		spell->effects[index]->baseEffect->data.taperCurve = tapCurve;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect taperCurve set to: {}", tapCurve);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectCastingSoundLevel(RE::StaticFunctionTag* base, RE::SpellItem* spell, int level, int index) {
		const auto type = static_cast<RE::SOUND_LEVEL>(level);
		spell->effects[index]->baseEffect->data.castingSoundLevel = type;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect sound level set to: {}", level);
		modifiedSpells.insert(spell);
	}
	static void setNthEffectDualCastScale(RE::StaticFunctionTag* base, RE::SpellItem* spell, float scale, int index) {
		spell->effects[index]->baseEffect->data.dualCastScale = scale;

		logger::info("For the spell: {}", spell->fullName.c_str());
		logger::info("Effect index is: {}", index);
		logger::info("Effect dual cast scale set to: {}", scale);
		modifiedSpells.insert(spell);
	}
		//Getters
	
	float getNthEffectSecondaryAVWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return spell->effects[index]->baseEffect->data.secondAVWeight;
	}
	float getNthEffectTaperDuration(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return spell->effects[index]->baseEffect->data.taperDuration;
	}
	float getNthEffectTaperWeight(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return spell->effects[index]->baseEffect->data.taperWeight;
	}
	float getNthEffectTaperCurve(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return spell->effects[index]->baseEffect->data.taperCurve;
	}
	int getNthEffectCastingSoundLevel(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return stl::to_underlying(spell->effects[index]->baseEffect->data.castingSoundLevel);
	}
	float getNthEffectDualCastScale(RE::StaticFunctionTag* base, RE::SpellItem* spell, int index) {
		return spell->effects[index]->baseEffect->data.dualCastScale;
	}

	//Register the functions
    bool RegisterFuncs(RE::BSScript::IVirtualMachine* registry) {
        registry->RegisterFunction("setChargeTime", "_SE_SpellExtender", SpellExtenderNamespace::setChargeTime, registry);
        registry->RegisterFunction("setChargeTimeAll", "_SE_SpellExtender", SpellExtenderNamespace::setChargeTimeAll, registry);
		registry->RegisterFunction("setSpellCost", "_SE_SpellExtender", SpellExtenderNamespace::setSpellCost, registry);
		registry->RegisterFunction("setSpellFlag", "_SE_SpellExtender", SpellExtenderNamespace::setSpellFlag, registry);
		registry->RegisterFunction("IsManualCalc", "_SE_SpellExtender", SpellExtenderNamespace::IsManualCalc, registry);
		registry->RegisterFunction("IgnoresResistance", "_SE_SpellExtender", SpellExtenderNamespace::IgnoresResistance, registry);
		registry->RegisterFunction("IgnoresLOS", "_SE_SpellExtender", SpellExtenderNamespace::IgnoresLOS, registry);
		registry->RegisterFunction("IsNoAbsorb", "_SE_SpellExtender", SpellExtenderNamespace::IsNoAbsorb, registry);
		registry->RegisterFunction("IsNoDualCastMod", "_SE_SpellExtender", SpellExtenderNamespace::IsNoDualCastMod, registry);
		registry->RegisterFunction("IsPCStartSpell", "_SE_SpellExtender", SpellExtenderNamespace::IsPCStartSpell, registry);
		registry->RegisterFunction("setNthEffectDelivery", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectDelivery, registry);
		registry->RegisterFunction("setNthEffectArchetype", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectArchetype, registry);
		registry->RegisterFunction("setNthEffectPrimaryAV", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectPrimaryAV, registry);
		registry->RegisterFunction("setNthEffectSecondaryAV", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectSecondaryAV, registry);
		registry->RegisterFunction("setNthEffectSecondaryAVWeight", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectSecondaryAVWeight, registry);
		registry->RegisterFunction("setNthEffectTaperValues", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectTaperValues, registry);
		registry->RegisterFunction("setNthEffectTaperCurve", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectTaperCurve, registry);
		registry->RegisterFunction("setNthEffectTaperDuration", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectTaperDuration, registry);
		registry->RegisterFunction("setNthEffectTaperWeight", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectTaperWeight, registry);
		registry->RegisterFunction("setNthEffectCastingSoundLevel", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectCastingSoundLevel, registry);
		registry->RegisterFunction("setNthEffectDualCastScale", "_SE_SpellExtender", SpellExtenderNamespace::setNthEffectDualCastScale, registry);
		registry->RegisterFunction("getNthEffectSecondaryAVWeight", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectSecondaryAVWeight, registry);
		registry->RegisterFunction("getNthEffectTaperCurve", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectTaperCurve, registry);
		registry->RegisterFunction("getNthEffectTaperDuration", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectTaperDuration, registry);
		registry->RegisterFunction("getNthEffectTaperWeight", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectTaperWeight, registry);
		registry->RegisterFunction("getNthEffectCastingSoundLevel", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectCastingSoundLevel, registry);
		registry->RegisterFunction("getNthEffectDualCastScale", "_SE_SpellExtender", SpellExtenderNamespace::getNthEffectDualCastScale, registry);
		registry->RegisterFunction("copySpell", "_SE_SpellExtender", SpellExtenderNamespace::copySpell, registry);
		registry->RegisterFunction("combineSpells", "_SE_SpellExtender", SpellExtenderNamespace::combineSpells, registry);
		return true;
    }

	// serializes and saves the data in our modified spells set
	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		if (a_intfc->OpenRecord('SP3L', 1)) {
			std::size_t size = modifiedSpells.size();
			logger::info("Saving {} spells", size);
			if (a_intfc->WriteRecordData(&size, sizeof(size))) {
				for (RE::SpellItem* elem : modifiedSpells) {
					SpellSerializer::serializeSpell(a_intfc, elem);
					logger::info("Spell {} saved", elem->fullName.c_str());
				}
			}
		} else {
			logger::error("Failed to open record for spells!");
		}
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		std::uint32_t type, version, length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			switch (type) {
			case 'SP3L':
				{
					std::size_t size;
					a_intfc->ReadRecordData(&size, sizeof(size));
					logger::info("{} elements to load", size);
					for (uint32_t i = 0; i < size; ++i) {
						RE::FormID id;
						if (a_intfc->ReadRecordData(&id, sizeof(id))) {
							RE::FormID newId;
							if (!a_intfc->ResolveFormID(id, newId)) {
								logger::warn("ID {} could not be found after loading the save", id);
								continue;
							}
							RE::SpellItem* spell = RE::TESForm::LookupByID<RE::SpellItem>(id);
							if (spell) {
								logger::info("attempting to load id {}, {}", spell->formID, spell->fullName.c_str());
								SpellSerializer::deserializeSpell(a_intfc, spell);
								modifiedSpells.insert(spell);
								logger::info("successfully loaded id {}, {}", spell->formID, spell->fullName.c_str());
							} else {
								logger::error("no spell loaded");
								break;
							}
						} else {
							logger::error("Couldn't resolve form id, maybe something deleted it?");
							break;
						}
					}
				}
				break;
			default:
				logger::error("Unrecognized signature type!");
				break;
			}
		}
	}
}
