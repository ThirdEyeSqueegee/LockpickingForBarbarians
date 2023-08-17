#pragma once

class Utility {
protected:
    Utility() = default;
    ~Utility() = default;

public:
    Utility(const Utility&) = delete;            // Prevent copy construction
    Utility(Utility&&) = delete;                 // Prevent move construction
    Utility& operator=(const Utility&) = delete; // Prevent copy assignment
    Utility& operator=(Utility&&) = delete;      // Prevent move assignment

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static RE::TESConditionItem cond_item;
    inline static RE::TESCondition is_power_attacking;
    inline static RE::ActorValue av_to_use;
    inline static RE::TESObjectWEAP* unarmed_weapon;

    static void InitUtility() {
        cond_item.data.comparisonValue.f = 1.0f;
        cond_item.data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPowerAttacking;

        is_power_attacking.head = &cond_item;
        logger::debug("Initialized power attack condition");

        av_to_use = Settings::h2h_present ? RE::ActorValue::kPickpocket : RE::ActorValue::kLockpicking;
        logger::debug("Initialized actor value");

        const auto handler = RE::TESDataHandler::GetSingleton();
        unarmed_weapon = handler->LookupForm<RE::TESObjectWEAP>(0x1f4, "Skyrim.esm"sv);
        logger::debug("Cached unarmed weapon");
    }
};
