#pragma once
#include <string>

enum class GameMode {
    TwoVTwo,
    FastDraw,
    Raid
};

enum class GameLocation {
    CastleTown,
    AirBase,
    SavageLand
};

enum class Rank {
    Bronze,
    Silver,
    Gold,
    Platinum
};

enum class MatchingType {
    SameRank,
    AnyRank
};

inline size_t getRequiredPlayers(GameMode mode) {
    switch (mode) {
        case GameMode::TwoVTwo: return 4;
        case GameMode::FastDraw: return 2;
        case GameMode::Raid: return 6;
        default: return 0;
    }
}

inline std::string gameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::TwoVTwo: return "TwoVTwo";
        case GameMode::FastDraw: return "FastDraw";
        case GameMode::Raid: return "Raid";
        default: return "Unknown";
    }
}

inline std::string gameLocationToString(GameLocation location) {
    switch (location) {
        case GameLocation::CastleTown: return "CastleTown";
        case GameLocation::AirBase: return "AirBase";
        case GameLocation::SavageLand: return "SavageLand";
        default: return "Unknown";
    }
}

inline std::string rankToString(Rank rank) {
    switch (rank) {
        case Rank::Bronze: return "Bronze";
        case Rank::Silver: return "Silver";
        case Rank::Gold: return "Gold";
        case Rank::Platinum: return "Platinum";
        default: return "Unknown";
    }
}

inline std::string matchingTypeToString(MatchingType type) {
    switch (type) {
        case MatchingType::SameRank: return "SameRank";
        case MatchingType::AnyRank: return "AnyRank";
        default: return "Unknown";
    }
}