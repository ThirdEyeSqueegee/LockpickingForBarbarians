#include "Events.h"

#include "Utility.h"
#include "Settings.h"
#include "Hooks.h"

namespace Events {
    OnHitEventHandler* OnHitEventHandler::GetSingleton() {
        static OnHitEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event,
                                                             RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->cause) {
            if (a_event->cause->IsPlayerRef()) {
                if (const auto locked = a_event->target->As<RE::TESObjectREFR>(); locked->IsLocked()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton();
                        Utility::is_power_attacking(player->AsReference(), locked)) {
                        if (const auto equipped_right = player->GetEquippedObject(false)) {
                            if (const auto weapon = equipped_right->As<RE::TESObjectWEAP>()) {
                                if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword()) {
                                    if (const auto lock = locked->GetLock()) {
                                        const auto level = lock->GetLockLevel(locked);
                                        const auto stamina_av = player->AsActorValueOwner()->GetActorValue(
                                            RE::ActorValue::kStamina
                                            );
                                        const auto two_handed_av = player->AsActorValueOwner()->GetActorValue(
                                            RE::ActorValue::kTwoHanded
                                            );
                                        const auto stamina_pct = Hooks::GetActorValuePercent(
                                            player->As<RE::Actor>(), RE::ActorValue::kStamina
                                            );
                                        const auto two_handed_pct = Hooks::GetActorValuePercent(
                                            player->As<RE::Actor>(), RE::ActorValue::kTwoHanded
                                            );
                                        const auto stamina = stamina_av / stamina_pct;
                                        const auto two_handed = two_handed_av / two_handed_pct;

                                        const auto av_to_increase = Settings::h2h_present
                                                                        ? RE::ActorValue::kPickpocket
                                                                        : RE::ActorValue::kLockpicking;

                                        if (Settings::use_stamina && Settings::use_2h) {
                                            if (stamina >= 100.0f && two_handed >= 20.0f
                                                && level == RE::LOCK_LEVEL::kVeryEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 17.5f);
                                            }
                                            if (stamina > 125.0f && two_handed > 30.0f
                                                && level == RE::LOCK_LEVEL::kEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 25.0f);
                                            }
                                            if (stamina > 175.0f && two_handed > 40.0f
                                                && level == RE::LOCK_LEVEL::kAverage) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 40.0f);
                                            }
                                            if (stamina > 225.0f && two_handed > 60.0f
                                                && level == RE::LOCK_LEVEL::kHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 80.0f);
                                            }
                                            if (stamina >= 275.0f && two_handed >= 80.0f
                                                && level == RE::LOCK_LEVEL::kVeryHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 120.0f);
                                            }
                                        }

                                        if (!Settings::use_stamina && Settings::use_2h) {
                                            if (two_handed >= 20.0f && level == RE::LOCK_LEVEL::kVeryEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 17.5f);
                                            }
                                            if (two_handed > 30.0f && level == RE::LOCK_LEVEL::kEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 25.0f);
                                            }
                                            if (two_handed > 40.0f && level == RE::LOCK_LEVEL::kAverage) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 40.0f);
                                            }
                                            if (two_handed > 60.0f && level == RE::LOCK_LEVEL::kHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 80.0f);
                                            }
                                            if (two_handed >= 80.0f && level == RE::LOCK_LEVEL::kVeryHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 120.0f);
                                            }
                                        }

                                        if (Settings::use_stamina && !Settings::use_2h) {
                                            if (stamina >= 100.0f && level == RE::LOCK_LEVEL::kVeryEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 17.5f);
                                            }
                                            if (stamina > 125.0f && level == RE::LOCK_LEVEL::kEasy) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 25.0f);
                                            }
                                            if (stamina > 175.0f && level == RE::LOCK_LEVEL::kAverage) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 40.0f);
                                            }
                                            if (stamina > 225.0f && level == RE::LOCK_LEVEL::kHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 80.0f);
                                            }
                                            if (stamina >= 275.0f && level == RE::LOCK_LEVEL::kVeryHard) {
                                                lock->SetLocked(false);
                                                player->AddSkillExperience(av_to_increase, 120.0f);
                                            }
                                        }
                                    }
                                }
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
