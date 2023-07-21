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

                                        auto cond1 = false, cond2 = false, cond3 = false, cond4 = false, cond5 = false;

                                        if (Settings::use_stamina && Settings::use_2h) {
                                            cond1 = stamina >= 100.0f && two_handed >= 20.0f
                                                    && level == RE::LOCK_LEVEL::kVeryEasy;
                                            cond2 = stamina > 125.0f && two_handed > 30.0f
                                                    && level == RE::LOCK_LEVEL::kEasy;
                                            cond3 = stamina > 175.0f && two_handed > 40.0f
                                                    && level == RE::LOCK_LEVEL::kAverage;
                                            cond4 = stamina > 225.0f && two_handed > 60.0f
                                                    && level == RE::LOCK_LEVEL::kHard;
                                            cond5 = stamina >= 275.0f && two_handed >= 80.0f
                                                    && level == RE::LOCK_LEVEL::kVeryHard;
                                        }

                                        if (!Settings::use_stamina && Settings::use_2h) {
                                            cond1 = two_handed >= 20.0f
                                                    && level == RE::LOCK_LEVEL::kVeryEasy;
                                            cond2 = two_handed > 30.0f
                                                    && level == RE::LOCK_LEVEL::kEasy;
                                            cond3 = two_handed > 40.0f
                                                    && level == RE::LOCK_LEVEL::kAverage;
                                            cond4 = two_handed > 60.0f
                                                    && level == RE::LOCK_LEVEL::kHard;
                                            cond5 = two_handed >= 80.0f
                                                    && level == RE::LOCK_LEVEL::kVeryHard;
                                        }

                                        if (Settings::use_stamina && !Settings::use_2h) {
                                            cond1 = stamina >= 100.0f
                                                    && level == RE::LOCK_LEVEL::kVeryEasy;
                                            cond2 = stamina > 125.0f
                                                    && level == RE::LOCK_LEVEL::kEasy;
                                            cond3 = stamina > 175.0f
                                                    && level == RE::LOCK_LEVEL::kAverage;
                                            cond4 = stamina > 225.0f
                                                    && level == RE::LOCK_LEVEL::kHard;
                                            cond5 = stamina >= 275.0f
                                                    && level == RE::LOCK_LEVEL::kVeryHard;
                                        }

                                        if (cond1) {
                                            lock->SetLocked(false);
                                            Hooks::FinalizeUnlock(locked->AsReference());
                                            player->UpdateCrosshairs();
                                            RE::PlaySound("UILockpickingUnlock");
                                            if (locked->IsCrimeToActivate())
                                                player->StealAlarm(locked->AsReference(),
                                                                   locked->As<RE::TESForm>(),
                                                                   0,
                                                                   100,
                                                                   locked->GetOwner(),
                                                                   false);
                                            player->AddSkillExperience(av_to_increase, 17.5f);
                                        }
                                        if (cond2) {
                                            lock->SetLocked(false);
                                            Hooks::FinalizeUnlock(locked->AsReference());
                                            player->UpdateCrosshairs();
                                            RE::PlaySound("UILockpickingUnlock");
                                            if (locked->IsCrimeToActivate())
                                                player->StealAlarm(locked->AsReference(),
                                                                   locked->As<RE::TESForm>(),
                                                                   0,
                                                                   200,
                                                                   locked->GetOwner(),
                                                                   false);
                                            player->AddSkillExperience(av_to_increase, 25.0f);
                                        }
                                        if (cond3) {
                                            lock->SetLocked(false);
                                            Hooks::FinalizeUnlock(locked->AsReference());
                                            player->UpdateCrosshairs();
                                            RE::PlaySound("UILockpickingUnlock");
                                            if (locked->IsCrimeToActivate())
                                                player->StealAlarm(locked->AsReference(),
                                                                   locked->As<RE::TESForm>(),
                                                                   0,
                                                                   400,
                                                                   locked->GetOwner(),
                                                                   false);
                                            player->AddSkillExperience(av_to_increase, 40.0f);
                                        }
                                        if (cond4) {
                                            lock->SetLocked(false);
                                            Hooks::FinalizeUnlock(locked->AsReference());
                                            player->UpdateCrosshairs();
                                            RE::PlaySound("UILockpickingUnlock");
                                            if (locked->IsCrimeToActivate())
                                                player->StealAlarm(locked->AsReference(),
                                                                   locked->As<RE::TESForm>(),
                                                                   0,
                                                                   800,
                                                                   locked->GetOwner(),
                                                                   false);
                                            player->AddSkillExperience(av_to_increase, 80.0f);
                                        }
                                        if (cond5) {
                                            lock->SetLocked(false);
                                            Hooks::FinalizeUnlock(locked->AsReference());
                                            player->UpdateCrosshairs();
                                            RE::PlaySound("UILockpickingUnlock");
                                            if (locked->IsCrimeToActivate())
                                                player->StealAlarm(locked->AsReference(),
                                                                   locked->As<RE::TESForm>(),
                                                                   0,
                                                                   1200,
                                                                   locked->GetOwner(),
                                                                   false);
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

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnHitEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered hit event handler");
    }
}
