#pragma once

class Settings {
protected:
    Settings() = default;
    ~Settings() = default;

public:
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

    static Settings* GetSingleton();

    static void LoadSettings();

    inline static bool debug_logging = false;

    inline static bool use_stamina = true;
    inline static bool use_2h = true;
    inline static bool h2h_present = false;
};
