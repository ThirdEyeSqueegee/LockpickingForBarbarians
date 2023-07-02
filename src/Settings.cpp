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

    use_stamina = ini.GetValue("General", "bUseStamina");
    use_2h = ini.GetValue("General", "bUse2H");

    if (RE::TESDataHandler::GetSingleton()->LookupLoadedModByName("HandtoHand.esp"sv) != nullptr)
        h2h_present = true;

    logger::info("Loaded settings");
}
