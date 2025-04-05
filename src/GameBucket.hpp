#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "Player.hpp"
#include "Enums.hpp"

class GameBucket {
private:
    GameMode mode;
    GameLocation location;
    std::optional<Rank> rank;
    MatchingType matchingType;
    std::vector<std::shared_ptr<Player>> waitingPlayers;

public:
    // Constructor for SameRank buckets
    GameBucket(GameMode m, GameLocation loc, Rank r, MatchingType type)
        : mode(m), location(loc), rank(r), matchingType(type) {}

    // Constructor for AnyRank buckets
    GameBucket(GameMode m, GameLocation loc, MatchingType type)
        : mode(m), location(loc), matchingType(type) {}

    void addPlayer(std::shared_ptr<Player> player) {
        waitingPlayers.push_back(player);
    }

    bool isReadyToMatch() const {
        return waitingPlayers.size() >= getRequiredPlayers(mode);
    }

    std::vector<std::shared_ptr<Player>> getMatchedPlayers() {
        std::vector<std::shared_ptr<Player>> matchedPlayers;
        size_t requiredPlayers = getRequiredPlayers(mode);
        
        for (size_t i = 0; i < requiredPlayers && i < waitingPlayers.size(); ++i) {
            matchedPlayers.push_back(waitingPlayers[i]);
        }
        
        waitingPlayers.erase(waitingPlayers.begin(), waitingPlayers.begin() + matchedPlayers.size());
        return matchedPlayers;
    }

    void removePlayersByIndices(const std::vector<size_t>& indices) {
        for (size_t index : indices) {
            if (index < waitingPlayers.size()) {
                waitingPlayers.erase(waitingPlayers.begin() + index);
            }
        }
    }

    // Getters
    GameMode getMode() const { return mode; }
    GameLocation getLocation() const { return location; }
    std::optional<Rank> getRank() const { return rank; }
    MatchingType getMatchingType() const { return matchingType; }
    const std::vector<std::shared_ptr<Player>>& getWaitingPlayers() const { return waitingPlayers; }
    size_t getWaitingPlayersCount() const { return waitingPlayers.size(); }
};