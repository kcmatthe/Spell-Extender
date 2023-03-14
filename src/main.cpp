#include "SpellExtender.h"
#include "PCH.h"
#include "logger.h"
#include "skse/API.h"
#include "skse/SKSE.h"
#include "skse/Version.h"
#include <ShlObj.h>
#include "Serialization.h"

using namespace RE::BSScript;
using namespace SpellExtenderNamespace;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

void OnMessage(SKSE::MessagingInterface::Message* message) {}

void InitializePapyrus()
{
	logger::info("Initializing Papyrus binding...");
	if (GetPapyrusInterface()->Register(SpellExtenderNamespace::RegisterFuncs)) {
		logger::info("Papyrus functions bound.");
	} 
	else {
		logger::info("Failure to register Papyrus bindings.");
	}
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("_kmatt_");
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST_AE });
	v.UsesNoStructs(true);

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();

	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse);
	SetupLog();

	SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
	auto* serde = GetSerializationInterface();
	serde->SetUniqueID(_byteswap_ulong('SP3L'));
	serde->SetSaveCallback(SpellExtenderNamespace::SaveCallback);
	serde->SetLoadCallback(SpellExtenderNamespace::LoadCallback);


	InitializePapyrus();

	return true;
};
