#pragma once

namespace Hooks
{
    static float GetActorValuePercent(RE::Actor* a_actor, RE::ActorValue a_actorValue) noexcept
    {
        static REL::Relocation<decltype(&GetActorValuePercent)> func{ RELOCATION_ID(36347, 37337) };
        return func(a_actor, a_actorValue);
    }

    static bool FinalizeUnlock(RE::TESObjectREFR* a_ref) noexcept
    {
        static REL::Relocation<decltype(&FinalizeUnlock)> func{ RELOCATION_ID(19110, 19512) };
        return func(a_ref);
    }
} // namespace Hooks
