#include "Events.h"

#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"

constexpr auto very_easy = RE::LOCK_LEVEL::kVeryEasy;
constexpr auto easy = RE::LOCK_LEVEL::kEasy;
constexpr auto average = RE::LOCK_LEVEL::kAverage;
constexpr auto hard = RE::LOCK_LEVEL::kHard;
constexpr auto very_hard = RE::LOCK_LEVEL::kVeryHard;

namespace Events {
    OnHitEventHandler* OnHitEventHandler::GetSingleton() {
        static OnHitEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;

        if (a_event->cause) {
            if (a_event->cause->IsPlayerRef()) {
                if (const auto locked = a_event->target->As<RE::TESObjectREFR>(); locked->IsLocked()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton(); Utility::is_power_attacking(player->AsReference(), locked)) {
                        logger::debug("Power attacking lock: {}", locked->GetName());
                        if (const auto equipped_right = player->GetEquippedObject(false)) {
                            if (const auto weapon = equipped_right->As<RE::TESObjectWEAP>()) {
                                logger::debug("{} equipped in right hand", weapon ? weapon->GetName() : "Nothing");
                                if (const auto unarmed = weapon->GetFormID() == Utility::unarmed_weapon->GetFormID();
                                    weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || unarmed) {
                                    const auto lock = locked->GetLock();
                                    const auto level = lock->GetLockLevel(locked);
                                    logger::debug("Lock level: {}", static_cast<int>(level));
                                    const auto stamina_av = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
                                    const auto stamina_pct = Hooks::GetActorValuePercent(player->As<RE::Actor>(), RE::ActorValue::kStamina);
                                    const auto stamina = stamina_av / stamina_pct;
                                    const auto two_handed = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kTwoHanded);
                                    const auto lockpicking = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kLockpicking);
                                    const auto heavy_armor = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHeavyArmor);
                                    logger::debug("Current player stamina: {}", stamina);
                                    logger::debug("Player Two-Handed skill: {}", two_handed);
                                    logger::debug("Player Lockpicking skill: {}", lockpicking);
                                    logger::debug("Player Heavy Armor skill: {}", heavy_armor);
                                    auto cond1 = false, cond2 = false, cond3 = false, cond4 = false, cond5 = false;
                                    if (Settings::use_stamina && Settings::use_2h) {
                                        cond1 = stamina >= 100.0f && two_handed >= 20.0f && level == very_easy;
                                        cond2 = stamina >= 125.0f && two_handed >= 30.0f && level == easy;
                                        cond3 = stamina >= 175.0f && two_handed >= 40.0f && level == average;
                                        cond4 = stamina >= 225.0f && two_handed >= 60.0f && level == hard;
                                        cond5 = stamina >= 275.0f && two_handed >= 80.0f && level == very_hard;
                                    } else if (!Settings::use_stamina && Settings::use_2h) {
                                        cond1 = two_handed >= 20.0f && level == very_easy;
                                        cond2 = two_handed >= 30.0f && level == easy;
                                        cond3 = two_handed >= 40.0f && level == average;
                                        cond4 = two_handed >= 60.0f && level == hard;
                                        cond5 = two_handed >= 80.0f && level == very_hard;
                                    } else if (Settings::use_stamina && !Settings::use_2h) {
                                        cond1 = stamina >= 100.0f && level == very_easy;
                                        cond2 = stamina >= 125.0f && level == easy;
                                        cond3 = stamina >= 175.0f && level == average;
                                        cond4 = stamina >= 225.0f && level == hard;
                                        cond5 = stamina >= 275.0f && level == very_hard;
                                    }
                                    if (unarmed) {
                                        logger::debug("Player is unarmed");
                                        const auto second_av = Settings::h2h_present ? lockpicking : heavy_armor;
                                        if (Settings::use_stamina) {
                                            cond1 = stamina >= 100.0f && second_av >= 20.0f && level == very_easy;
                                            cond2 = stamina >= 125.0f && second_av >= 30.0f && level == easy;
                                            cond3 = stamina >= 175.0f && second_av >= 40.0f && level == average;
                                            cond4 = stamina >= 225.0f && second_av >= 60.0f && level == hard;
                                            cond5 = stamina >= 275.0f && second_av >= 80.0f && level == very_hard;
                                        } else {
                                            cond1 = second_av >= 20.0f && level == very_easy;
                                            cond2 = second_av >= 30.0f && level == easy;
                                            cond3 = second_av >= 40.0f && level == average;
                                            cond4 = second_av >= 60.0f && level == hard;
                                            cond5 = second_av >= 80.0f && level == very_hard;
                                        }
                                    }
                                    if (cond1)
                                        ProcessHit(lock, locked, 100, 17.5f);
                                    else if (cond2)
                                        ProcessHit(lock, locked, 200, 25.0f);
                                    else if (cond3)
                                        ProcessHit(lock, locked, 400, 40.0f);
                                    else if (cond4)
                                        ProcessHit(lock, locked, 800, 80.0f);
                                    else if (cond5)
                                        ProcessHit(lock, locked, 1200, 120.0f);
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

    void OnHitEventHandler::ProcessHit(RE::REFR_LOCK* lock, RE::TESObjectREFR* locked, int alarm_value, float xp_gain) {
        lock->SetLocked(false);
        logger::debug("Unlocked {}", locked->GetName());
        Hooks::FinalizeUnlock(locked->AsReference());
        logger::debug("Finalized unlock for {}", locked->GetName());
        const auto player = RE::PlayerCharacter::GetSingleton();
        player->UpdateCrosshairs();
        RE::PlaySound("UILockpickingUnlock");
        if (locked->IsCrimeToActivate()) {
            player->StealAlarm(locked->AsReference(), locked->As<RE::TESForm>(), 0, alarm_value, locked->GetOwner(), false);
            logger::debug("Sent steal alarm ({} gold)", alarm_value);
        }
        player->AddSkillExperience(Utility::av_to_use, xp_gain);
        logger::debug("Added {} {} XP", xp_gain, Settings::h2h_present ? "Security" : "Lockpicking");
    }
}
