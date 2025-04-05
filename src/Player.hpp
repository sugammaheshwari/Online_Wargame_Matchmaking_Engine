#pragma once
#include <string>
#include "Enums.hpp"

class Player {
private:
    std::string playerId;
    Rank rank;
    MatchingType matchingType;

public:
    Player(const std::string& id, Rank r, MatchingType type)
        : playerId(id), rank(r), matchingType(type) {}

    std::string getPlayerId() const { return playerId; }
    Rank getRank() const { return rank; }
    MatchingType getMatchingType() const { return matchingType; }
};