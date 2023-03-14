#pragma once

#include <RE/Skyrim.h>

namespace SpellSerializer
{
#pragma warning(push)
#pragma warning(disable : 4251)
	// reads a serialized form id and returns its pointer
	template <typename T>
	T* _deserializeNextForm(SKSE::SerializationInterface* a_intfc);

	// serializes a spell's effect item (so we can serialize the spell's effects)
	bool _serializeEffectItem(SKSE::SerializationInterface* a_intfc, RE::Effect* effect);

	// deserializes a spell's effect item
	RE::Effect* _deserializeEffectItem(SKSE::SerializationInterface* a_intfc);

	// fully serializes a spell
	bool serializeSpell(SKSE::SerializationInterface* a_intfc, RE::SpellItem* spell);

	// reads a serialized spell and remodifies it
	bool deserializeSpell(SKSE::SerializationInterface* a_intfc, RE::SpellItem* spell);
}
