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

    debug_logging = ini.GetValue("Log", "Debug");

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    use_stamina = ini.GetValue("General", "bUseStamina");
    use_2h = ini.GetValue("General", "bUse2H");

    if (RE::TESDataHandler::GetSingleton()->LookupLoadedModByName("HandtoHand.esp"sv))
        h2h_present = true;

    logger::info("Loaded settings");
    logger::info("\tbUseStamina = {}", use_stamina);
    logger::info("\tbUse2H = {}", use_2h);
}
