#pragma once

namespace Events
{
    class OnHitEventHandler : public EventSingleton<OnHitEventHandler, RE::TESHitEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) noexcept override;

        static void ProcessHit(RE::REFR_LOCK* lock, RE::TESObjectREFR* locked, int alarm_value, float xp_gain) noexcept;
    };
} // namespace Events
