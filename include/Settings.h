#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static bool use_stamina{};

    inline static bool use_weapon_skill{};

    inline static bool no_xp_gain{};

    inline static bool h2h_present{};
};
