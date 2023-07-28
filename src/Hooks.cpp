#include "Hooks.h"

namespace Hooks {
    float GetActorValuePercent(RE::Actor* a_actor, RE::ActorValue a_actorValue) {
        REL::Relocation<decltype(&GetActorValuePercent)> func{ RELOCATION_ID(36347, 37337) };
        logger::debug("Calling GetActorValuePercent hook");
        return func(a_actor, a_actorValue);
    }

    bool FinalizeUnlock(RE::TESObjectREFR* a_ref) {
        REL::Relocation<decltype(&FinalizeUnlock)> func{ RELOCATION_ID(19110, 19512) };
        logger::debug("Calling FinalizeUnlock hook");
        return func(a_ref);
    }
}
