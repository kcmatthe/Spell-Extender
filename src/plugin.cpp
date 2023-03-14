#include <ShlObj.h>
#include "PCH.h"
#include "skse/Version.h"
#include "logger.h"
#include "skse/API.h"
#include "skse/SKSE.h"
#include "ControlledCasting.h"


using namespace RE::BSScript;
using namespace ControlledCastingNamespace;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

void OnMessage(SKSE::MessagingInterface::Message *message) {    }

void InitializePapyrus() {
    logger:info("Initializing Papyrus binding...");
    if (GetPapyrusInterface()->Register(ControlledCastingNamespace::RegisterFuncs)) {
        logger:info("Papyrus functions bound.");
    } else {
        logger:info("Failure to register Papyrus bindings.");
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SetupLog();

    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);

    InitializePapyrus();

    return true;
};
