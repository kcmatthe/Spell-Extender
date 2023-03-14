#include "Serialization.h"

namespace SpellSerializer
{
	// reads a serialized form id and returns its pointer
	template <typename T>
	T* _deserializeNextForm(SKSE::SerializationInterface* a_intfc)
	{
		T* ret = NULL;
		RE::FormID id;
		a_intfc->ReadRecordData(&id, sizeof(id));
		if (a_intfc->ResolveFormID(id, id)) {
			ret = RE::TESForm::LookupByID<T>(id);
		}
		logger::info("deserialized form {}", id);
		return ret;
	}
	// reads a serialized form id and returns its pointer
	bool _serializeEffectItem(SKSE::SerializationInterface* a_intfc, RE::Effect* effect)
	{
		a_intfc->WriteRecordData(&(effect->effectItem.magnitude), sizeof(float));
		a_intfc->WriteRecordData(&(effect->effectItem.area), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(effect->effectItem.duration), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(effect->pad0C), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(effect->cost), sizeof(float));
		a_intfc->WriteRecordData(&(effect->pad1C), sizeof(uint32_t));
		//a_intfc->WriteRecordData(&(effect->baseEffect), sizeof(RE::EffectSetting*));
		//a_intfc->WriteRecordData(&(effect->baseEffect->data), sizeof(RE::EffectSetting::EffectSettingData));
		logger::info("serialize effect: {}, {}", effect->baseEffect->formID, effect->baseEffect->fullName.c_str());
		a_intfc->WriteRecordData(&(effect->baseEffect->formID), sizeof(RE::FormID));
		
		// serialize name in case it was modified
		std::size_t len = strlen(effect->baseEffect->fullName.c_str());
		a_intfc->WriteRecordData(&len, sizeof(len));
		a_intfc->WriteRecordData(effect->baseEffect->fullName.c_str(), len);

		//data
		a_intfc->WriteRecordData(&(effect->baseEffect->data.flags), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.baseCost), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.associatedForm), sizeof(RE::TESForm));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.associatedSkill), sizeof(RE::ActorValue));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.resistVariable), sizeof(RE::ActorValue));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.taperWeight), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.taperCurve), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.taperDuration), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.minimumSkill), sizeof(int32_t));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.spellmakingArea), sizeof(int32_t));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.spellmakingChargeTime), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.secondaryAV), sizeof(RE::ActorValue));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.secondAVWeight), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.archetype), sizeof(RE::EffectArchetypes));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.primaryAV), sizeof(RE::ActorValue));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.castingType), sizeof(RE::MagicSystem::CastingType));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.delivery), sizeof(RE::MagicSystem::Delivery));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.skillUsageMult), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.dualCastScale), sizeof(float));
		a_intfc->WriteRecordData(&(effect->baseEffect->data.castingSoundLevel), sizeof(RE::SOUND_LEVEL));
		
		return true;
	}

	RE::Effect* _deserializeEffectItem(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("attempting to deserialize effect");
		RE::Effect* effect = new RE::Effect();
		a_intfc->ReadRecordData(&(effect->effectItem.magnitude), sizeof(float));
		a_intfc->ReadRecordData(&(effect->effectItem.area), sizeof(uint32_t));
		a_intfc->ReadRecordData(&(effect->effectItem.duration), sizeof(uint32_t));
		a_intfc->ReadRecordData(&(effect->pad0C), sizeof(uint32_t));
		a_intfc->ReadRecordData(&(effect->cost), sizeof(float));
		a_intfc->ReadRecordData(&(effect->pad1C), sizeof(uint32_t));
		logger::info("deserialized effect data, moving onto base effect");

		effect->baseEffect = _deserializeNextForm<RE::EffectSetting>(a_intfc);
		/*
		RE::FormID id;
		RE::EffectSetting* mgef = NULL;
		a_intfc->ReadRecordData(&id, sizeof(id));
		if (a_intfc->ResolveFormID(id, id)) {
			mgef = RE::TESForm::LookupByID<RE::EffectSetting>(id);
			effect->baseEffect = mgef;
			logger::info("deserialized form {}", id);
		}
		*/
		
		
		std::size_t len;
		a_intfc->ReadRecordData(&len, sizeof(len));
		char buf[257] = { '\0' };
		a_intfc->ReadRecordData(buf, len);
		effect->baseEffect->fullName = buf;

		logger::info("effect name {}", effect->baseEffect->fullName.c_str());
			
		a_intfc->ReadRecordData(&(effect->baseEffect->data.flags), sizeof(uint32_t));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.baseCost), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.associatedForm), sizeof(RE::TESForm));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.associatedSkill), sizeof(RE::ActorValue));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.resistVariable), sizeof(RE::ActorValue));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.taperWeight), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.taperCurve), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.taperDuration), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.minimumSkill), sizeof(int32_t));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.spellmakingArea), sizeof(int32_t));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.spellmakingChargeTime), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.secondaryAV), sizeof(RE::ActorValue));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.secondAVWeight), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.archetype), sizeof(RE::EffectArchetypes));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.primaryAV), sizeof(RE::ActorValue));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.castingType), sizeof(RE::MagicSystem::CastingType));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.delivery), sizeof(RE::MagicSystem::Delivery));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.skillUsageMult), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.dualCastScale), sizeof(float));
		a_intfc->ReadRecordData(&(effect->baseEffect->data.castingSoundLevel), sizeof(RE::SOUND_LEVEL));

		logger::info("Deserialized effect {} ({})", effect->baseEffect->fullName.c_str(), effect->baseEffect->RE::TESForm::GetFormID());

		return effect;
	}

	bool serializeSpell(SKSE::SerializationInterface* a_intfc, RE::SpellItem* spell)
	{
		// serialize form id so we can find the spell again during load
		logger::info("{}", spell->formID);
		a_intfc->WriteRecordData(&(spell->formID), sizeof(uint32_t));

		// serialize name in case it was modified
		std::size_t len = strlen(spell->fullName.c_str());
		a_intfc->WriteRecordData(&len, sizeof(len));
		a_intfc->WriteRecordData(spell->fullName.c_str(), len);

		// serialize other stuff
		a_intfc->WriteRecordData(&(spell->keywords), sizeof(RE::BGSKeyword));
		a_intfc->WriteRecordData(&(spell->hostileCount), sizeof(int32_t));
		a_intfc->WriteRecordData(&(spell->equipSlot->formID), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(spell->menuDispObject->formID), sizeof(uint32_t));

		// serialize the spell's effects
		//need to get the read and writes to line up. Currently struggling to read the correct # of magic effects
		uint32_t num_effects = spell->effects.size();
		a_intfc->WriteRecordData((&num_effects), sizeof(uint32_t));
		logger::info("saving {} effects", num_effects);
		//a_intfc->WriteRecordData(&(spell->effects.size()), sizeof(uint32_t));
			for (auto& effect : spell->effects) 
			{
			_serializeEffectItem(a_intfc, effect);
			logger::info("Serialized effect {}", effect->baseEffect->fullName.c_str());
			}
		
		logger::info("{} effects serialized", spell->effects.size());
		// serialize the data struct
		a_intfc->WriteRecordData(&(spell->data), sizeof(RE::SpellItem::Data));
		a_intfc->WriteRecordData(&(spell->data.spellType), sizeof(RE::MagicSystem::SpellType));
		a_intfc->WriteRecordData(&(spell->data.chargeTime), sizeof(float));
		a_intfc->WriteRecordData(&(spell->data.castingType), sizeof(RE::MagicSystem::CastingType));
		a_intfc->WriteRecordData(&(spell->data.range), sizeof(float));
		a_intfc->WriteRecordData(&(spell->data.castDuration), sizeof(float));  
		a_intfc->WriteRecordData(&(spell->data.delivery), sizeof(RE::MagicSystem::Delivery));
		a_intfc->WriteRecordData(&(spell->data.costOverride), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(spell->data.flags), sizeof(uint32_t));
		a_intfc->WriteRecordData(&(spell->data.castingPerk->formID), sizeof(uint32_t));

		logger::info("successfully saved spell {}", spell->fullName.c_str());

		return true;
	}

	bool deserializeSpell(SKSE::SerializationInterface* a_intfc, RE::SpellItem* spell)
	{
		logger::info("beginning deserialization of {}", spell->fullName.c_str());

		std::size_t len;
		a_intfc->ReadRecordData(&len, sizeof(len));
		char buf[257] = { '\0' };
		a_intfc->ReadRecordData(buf, len);
		spell->fullName = buf;

		logger::info("deserialized name ({})", spell->fullName.c_str());

		a_intfc->ReadRecordData(&(spell->keywords), sizeof(RE::BGSKeyword));
		a_intfc->ReadRecordData(&(spell->hostileCount), sizeof(int32_t));
		//a_intfc->ReadRecordData(&(spell->equipSlot), sizeof(RE::BGSEquipSlot));
		//a_intfc->ReadRecordData(&(spell->menuDispObject), sizeof(RE::TESBoundObject));

		spell->equipSlot = _deserializeNextForm<RE::BGSEquipSlot>(a_intfc);
		spell->menuDispObject = _deserializeNextForm<RE::TESBoundObject>(a_intfc);
		
		logger::info("deserialized keywords, equip slot, and related");
		
		logger::info("now attempting to begin effect deserialization");
		
		uint32_t num_effects;
		if (a_intfc->ReadRecordData((&num_effects), sizeof(uint32_t))) {
			spell->effects.clear();
			logger::info("number of effects loading: {}", num_effects);
			for (int i = 0; i < num_effects; ++i) {
				//_deserializeEffectItem(a_intfc);
				spell->effects.push_back(_deserializeEffectItem(a_intfc));
			}
			logger::info("Loaded {} effects", spell->effects.size());
		}
		
		logger::info("beginning deserialization of other spell data");

		a_intfc->ReadRecordData(&(spell->data), sizeof(RE::SpellItem::Data));
		a_intfc->ReadRecordData(&(spell->data.spellType), sizeof(RE::MagicSystem::SpellType));
		a_intfc->ReadRecordData(&(spell->data.chargeTime), sizeof(float));
		a_intfc->ReadRecordData(&(spell->data.castingType), sizeof(RE::MagicSystem::CastingType));
		a_intfc->ReadRecordData(&(spell->data.range), sizeof(float));
		a_intfc->ReadRecordData(&(spell->data.castDuration), sizeof(float)); 
		a_intfc->ReadRecordData(&(spell->data.delivery), sizeof(RE::MagicSystem::Delivery));
		a_intfc->ReadRecordData(&(spell->data.costOverride), sizeof(uint32_t));
		a_intfc->ReadRecordData(&(spell->data.flags), sizeof(uint32_t));
		//a_intfc->ReadRecordData(&(spell->data.castingPerk), sizeof(RE::BGSPerk));
		spell->data.castingPerk = _deserializeNextForm<RE::BGSPerk>(a_intfc);
		logger::info("successfully loaded spell {}", spell->fullName.c_str());

		return true;
	}
}
