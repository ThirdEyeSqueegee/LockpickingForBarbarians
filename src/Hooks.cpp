#include "Hooks.h"

namespace Hooks {
    float GetActorValuePercent(RE::Actor* a_actor, RE::ActorValue a_actorValue) {
        REL::Relocation<decltype(&GetActorValuePercent)> func{ RELOCATION_ID(36347, 37337) };
        return func(a_actor, a_actorValue);
    }
}