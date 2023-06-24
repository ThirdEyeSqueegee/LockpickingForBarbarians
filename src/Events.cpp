#include "Events.h"

#include "Utility.h"

namespace Events {
    OnHitEventHandler* OnHitEventHandler::GetSingleton() {
        static OnHitEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event,
                                                             RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->cause->IsPlayerRef()) {
            if (const auto chest = a_event->target->As<RE::TESObjectREFR>(); chest->IsLocked()) {
                if (const auto player = RE::PlayerCharacter::GetSingleton();
                    Utility::is_power_attacking(player->AsReference(), chest)) {
                    const auto equipped_right = player->GetEquippedObject(false);
                    if (const auto weapon = equipped_right->As<RE::TESObjectWEAP>();
                        weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword()) {
                        if (const auto lock = chest->GetLock(); lock->IsLocked()) {
                            const auto level = lock->GetLockLevel(chest);
                            const auto stamina = player->AsActorValueOwner()->GetActorValue(
                                RE::ActorValue::kStamina
                            );
                            if (stamina > 100.0f && stamina <= 125.0f && level == RE::LOCK_LEVEL::kVeryEasy) {
                                lock->SetLocked(false);
                                player->AddSkillExperience(RE::ActorValue::kStamina, 17.5f);
                            }
                            if (stamina > 125.0f && stamina <= 175.0f && level == RE::LOCK_LEVEL::kEasy) {
                                lock->SetLocked(false);
                                player->AddSkillExperience(RE::ActorValue::kStamina, 25.0f);
                            }
                            if (stamina > 175.0f && stamina <= 225.0f && level == RE::LOCK_LEVEL::kAverage) {
                                lock->SetLocked(false);
                                player->AddSkillExperience(RE::ActorValue::kStamina, 40.0f);
                            }
                            if (stamina > 225.0f && stamina <= 275.0f && level == RE::LOCK_LEVEL::kHard) {
                                lock->SetLocked(false);
                                player->AddSkillExperience(RE::ActorValue::kStamina, 80.0f);
                            }
                            if (stamina >= 275 && level == RE::LOCK_LEVEL::kVeryHard) {
                                lock->SetLocked(false);
                                player->AddSkillExperience(RE::ActorValue::kStamina, 120.0f);
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnHitEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered hit event handler");
    }
}
