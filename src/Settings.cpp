#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\LockpickingForBarbarians.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    use_stamina      = ini.GetBoolValue("General", "bUseStamina");
    use_weapon_skill = ini.GetBoolValue("General", "bUseWeaponSkill");

    if (!use_stamina && !use_weapon_skill) {
        logger::error("ERROR: At least one of bUseStamina and bUseWeaponSkill must be set to true in LockpickingForBarbarians.ini.");
        stl::report_and_fail("Error while loading settings for Lockpicking for Barbarians. See LockpickingForBarbarians.log for details.");
    }

    if (RE::TESDataHandler::GetSingleton()->LookupLoadedModByName("HandtoHand.esp"sv)) {
        h2h_present = true;
    }

    logger::info("Loaded settings");
    logger::info("\tbUseStamina = {}", use_stamina);
    logger::info("\tbUseWeaponSkill = {}", use_weapon_skill);
    logger::info("");
}
