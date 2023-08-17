#include "Settings.h"

#include "SimpleIni.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\LockpickingForBarbarians.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    use_stamina = ini.GetBoolValue("General", "bUseStamina");
    use_2h = ini.GetBoolValue("General", "bUse2H");

    if (!use_stamina && !use_2h) {
        logger::error("ERROR: At least one of bUseStamina and bUse2H must be set to true in LockpickingForBarbarians.ini.");
        SKSE::stl::report_and_fail("Error while loading settings for Lockpicking for Barbarians. See LockpickingForBarbarians.log for details.");
    }

    if (RE::TESDataHandler::GetSingleton()->LookupLoadedModByName("HandtoHand.esp"sv))
        h2h_present = true;

    logger::info("Loaded settings");
    logger::info("\tbUseStamina = {}", use_stamina);
    logger::info("\tbUse2H = {}", use_2h);
}
