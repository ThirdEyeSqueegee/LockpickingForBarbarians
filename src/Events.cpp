#include "Events.h"

#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"

constexpr auto very_easy{ RE::LOCK_LEVEL::kVeryEasy };
constexpr auto easy{ RE::LOCK_LEVEL::kEasy };
constexpr auto average{ RE::LOCK_LEVEL::kAverage };
constexpr auto hard{ RE::LOCK_LEVEL::kHard };
constexpr auto very_hard{ RE::LOCK_LEVEL::kVeryHard };

namespace Events
{
    RE::BSEventNotifyControl OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) noexcept
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!a_event->cause) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (a_event->cause->IsPlayerRef()) {
            if (const auto locked{ a_event->target->As<RE::TESObjectREFR>() }; locked && locked->IsLocked()) {
                if (const auto player{ RE::PlayerCharacter::GetSingleton() }; Utility::is_power_attacking(player->AsReference(), locked)) {
                    logger::debug("Power attacking lock: {}", locked->GetName());
                    if (const auto equipped_right{ player->GetEquippedObject(false) }) {
                        if (const auto weapon{ equipped_right->As<RE::TESObjectWEAP>() }) {
                            if (const auto unarmed{ weapon->GetFormID() == Utility::unarmed_weapon->GetFormID() };
                                weapon->IsOneHandedAxe() || weapon->IsOneHandedDagger() || weapon->IsOneHandedMace() || weapon->IsOneHandedSword() || weapon->IsTwoHandedAxe()
                                || weapon->IsTwoHandedSword() || unarmed)
                            {
                                logger::debug("{} equipped in right hand", unarmed ? weapon->GetName() : "Nothing");

                                const auto is_2h{ weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() };
                                const auto lock{ locked->GetLock() };
                                const auto level{ lock->GetLockLevel(locked) };
                                logger::debug("Lock level: {}", static_cast<int>(level));

                                const auto stamina_av{ player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) };
                                const auto stamina_pct{ Hooks::GetActorValuePercent(player->As<RE::Actor>(), RE::ActorValue::kStamina) };
                                const auto stamina{ stamina_av / stamina_pct };
                                const auto one_handed{ player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kOneHanded) };
                                const auto two_handed{ player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kTwoHanded) };
                                const auto lockpicking{ player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kLockpicking) };
                                const auto heavy_armor{ player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHeavyArmor) };

                                float av_to_use{};
                                if (is_2h) {
                                    av_to_use = two_handed;
                                }
                                else {
                                    av_to_use = one_handed;
                                }

                                logger::debug("Current player stamina: {}", stamina);
                                logger::debug("Player One-Handed skill: {}", one_handed);
                                logger::debug("Player Two-Handed skill: {}", two_handed);
                                logger::debug("Player Lockpicking skill: {}", lockpicking);
                                logger::debug("Player Heavy Armor skill: {}", heavy_armor);

                                bool cond1{}, cond2{}, cond3{}, cond4{}, cond5{};
                                if (Settings::use_stamina && Settings::use_weapon_skill) {
                                    cond1 = stamina >= 100.0f && av_to_use >= 20.0f && level == very_easy;
                                    cond2 = stamina >= 125.0f && av_to_use >= 30.0f && level == easy;
                                    cond3 = stamina >= 175.0f && av_to_use >= 40.0f && level == average;
                                    cond4 = stamina >= 225.0f && av_to_use >= 60.0f && level == hard;
                                    cond5 = stamina >= 275.0f && av_to_use >= 80.0f && level == very_hard;
                                }
                                else if (!Settings::use_stamina && Settings::use_weapon_skill) {
                                    cond1 = av_to_use >= 20.0f && level == very_easy;
                                    cond2 = av_to_use >= 30.0f && level == easy;
                                    cond3 = av_to_use >= 40.0f && level == average;
                                    cond4 = av_to_use >= 60.0f && level == hard;
                                    cond5 = av_to_use >= 80.0f && level == very_hard;
                                }
                                else if (Settings::use_stamina && !Settings::use_weapon_skill) {
                                    cond1 = stamina >= 100.0f && level == very_easy;
                                    cond2 = stamina >= 125.0f && level == easy;
                                    cond3 = stamina >= 175.0f && level == average;
                                    cond4 = stamina >= 225.0f && level == hard;
                                    cond5 = stamina >= 275.0f && level == very_hard;
                                }
                                if (unarmed) {
                                    logger::debug("Player is unarmed");
                                    const auto second_av{ Settings::h2h_present ? lockpicking : heavy_armor };
                                    if (Settings::use_stamina) {
                                        cond1 = stamina >= 100.0f && second_av >= 20.0f && level == very_easy;
                                        cond2 = stamina >= 125.0f && second_av >= 30.0f && level == easy;
                                        cond3 = stamina >= 175.0f && second_av >= 40.0f && level == average;
                                        cond4 = stamina >= 225.0f && second_av >= 60.0f && level == hard;
                                        cond5 = stamina >= 275.0f && second_av >= 80.0f && level == very_hard;
                                    }
                                    else {
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

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnHitEventHandler::ProcessHit(RE::REFR_LOCK* lock, RE::TESObjectREFR* locked, int alarm_value, float xp_gain) noexcept
    {
        lock->SetLocked(false);
        logger::debug("Unlocked {}", locked->GetName());

        Hooks::FinalizeUnlock(locked->AsReference());
        logger::debug("Finalized unlock for {}", locked->GetName());

        const auto player{ RE::PlayerCharacter::GetSingleton() };
        player->UpdateCrosshairs();
        RE::PlaySound("UILockpickingUnlock");
        if (locked->IsCrimeToActivate()) {
            player->StealAlarm(locked->AsReference(), locked->As<RE::TESForm>(), 0, alarm_value, locked->GetOwner(), false);
            logger::debug("Sent steal alarm ({} gold)", alarm_value);
        }
        player->AddSkillExperience(Utility::av_to_use, xp_gain);
        logger::debug("Added {} {} XP", xp_gain, Settings::h2h_present ? "Security" : "Lockpicking");
    }
} // namespace Events
