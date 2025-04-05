#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <vector>
#include <mutex>
#include "GameBucket.hpp"
#include "BucketFactory.hpp"

class MatchMakingEngine {
private:
    static std::unique_ptr<MatchMakingEngine> instance;
    static std::mutex mutex;
    std::unordered_map<std::string, std::unique_ptr<GameBucket>> buckets;
    MatchMakingEngine() = default;
    MatchMakingEngine(const MatchMakingEngine&) = delete;
    MatchMakingEngine& operator=(const MatchMakingEngine&) = delete;

    std::string getBucketKey(GameMode mode, GameLocation location, Rank rank, MatchingType type) const {
        return std::to_string(static_cast<int>(mode)) + "_" +
               std::to_string(static_cast<int>(location)) + "_" +
               std::to_string(static_cast<int>(rank)) + "_" +
               std::to_string(static_cast<int>(type));
    }

    std::string getAnyRankBucketKey(GameMode mode, GameLocation location) const {
        return std::to_string(static_cast<int>(mode)) + "_" +
               std::to_string(static_cast<int>(location)) + "_anyrank";
    }

    GameBucket* getOrCreateBucket(GameMode mode, GameLocation location, Rank rank, MatchingType type) {
        std::string key;
        if (type == MatchingType::AnyRank) {
            key = getAnyRankBucketKey(mode, location);
        } else {
            key = getBucketKey(mode, location, rank, type);
        }

        auto it = buckets.find(key);
        if (it == buckets.end()) {
            auto [inserted, success] = buckets.emplace(key, 
                BucketFactory::createBucket(mode, location, rank, type));
            return inserted->second.get();
        }
        return it->second.get();
    }

    bool tryFillWithAnyRankPlayers(GameBucket* bucket) {
        if (!bucket || bucket->getMatchingType() != MatchingType::SameRank || !bucket->getRank()) return false;

        auto* anyRankBucket = getOrCreateBucket(bucket->getMode(), bucket->getLocation(), 
                                              bucket->getRank().value(), MatchingType::AnyRank);
        
        size_t playersNeeded = getRequiredPlayers(bucket->getMode()) - bucket->getWaitingPlayersCount();
        if (playersNeeded <= 0) return false;

        size_t availableAnyRankPlayers = 0;
        for (const auto& player : anyRankBucket->getWaitingPlayers()) {
            if (player->getRank() == bucket->getRank().value()) {
                availableAnyRankPlayers++;
            }
        }

        if (availableAnyRankPlayers >= playersNeeded) {
            std::vector<size_t> movedPlayerIndices;
            size_t moved = 0;
            const auto& players = anyRankBucket->getWaitingPlayers();

            for (size_t i = 0; i < players.size() && moved < playersNeeded; ++i) {
                if (players[i]->getRank() == bucket->getRank().value()) {
                    bucket->addPlayer(players[i]);
                    movedPlayerIndices.push_back(i);
                    moved++;
                }
            }

            std::sort(movedPlayerIndices.begin(), movedPlayerIndices.end(), std::greater<size_t>());
            anyRankBucket->removePlayersByIndices(movedPlayerIndices);

            return true;
        }

        return false;
    }

    void tryCreateMatchWithAnyRank(GameBucket* bucket) {
        if (!bucket) return;

        if (bucket->isReadyToMatch()) {
            auto matchedPlayers = bucket->getMatchedPlayers();
            printMatchCreated(matchedPlayers, bucket);
            return;
        }

        if (tryFillWithAnyRankPlayers(bucket)) {
            auto matchedPlayers = bucket->getMatchedPlayers();
            printMatchCreated(matchedPlayers, bucket);
        }
    }

    void printMatchCreated(const std::vector<std::shared_ptr<Player>>& players, GameBucket* bucket) {
        std::cout << "Match created for " << players.size() << " players in mode: "
                 << gameModeToString(bucket->getMode())
                 << " location: " << gameLocationToString(bucket->getLocation())
                 << " matching type: " << matchingTypeToString(bucket->getMatchingType());
        
        if (bucket->getRank()) {
            std::cout << " rank: " << rankToString(bucket->getRank().value());
        }
        std::cout << std::endl;
        
        for (const auto& player : players) {
            std::cout << "  Player: " << player->getPlayerId() 
                     << " (Rank: " << rankToString(player->getRank()) 
                     << ", Type: " << matchingTypeToString(player->getMatchingType()) << ")" << std::endl;
        }
    }

public:
    static MatchMakingEngine& getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            instance = std::unique_ptr<MatchMakingEngine>(new MatchMakingEngine());
        }
        return *instance;
    }

    void addPlayer(std::shared_ptr<Player> player, GameMode mode, GameLocation location) {
        if (player->getMatchingType() == MatchingType::AnyRank) {
            auto* anyRankBucket = getOrCreateBucket(mode, location, player->getRank(), MatchingType::AnyRank);
            anyRankBucket->addPlayer(player);

            if (anyRankBucket->isReadyToMatch()) {
                auto matchedPlayers = anyRankBucket->getMatchedPlayers();
                printMatchCreated(matchedPlayers, anyRankBucket);
                return;
            }

            auto* sameRankBucket = getOrCreateBucket(mode, location, player->getRank(), MatchingType::SameRank);
            if (sameRankBucket->getWaitingPlayersCount() > 0) {
                tryCreateMatchWithAnyRank(sameRankBucket);
            }
        } else {
            auto* bucket = getOrCreateBucket(mode, location, player->getRank(), MatchingType::SameRank);
            bucket->addPlayer(player);
            tryCreateMatchWithAnyRank(bucket);
        }
    }

    size_t getWaitingPlayersCount(GameMode mode, GameLocation location, Rank rank, MatchingType type) const {
        std::string key;
        if (type == MatchingType::AnyRank) {
            key = getAnyRankBucketKey(mode, location);
        } else {
            key = getBucketKey(mode, location, rank, type);
        }
        
        auto it = buckets.find(key);
        if (it != buckets.end()) {
            return it->second->getWaitingPlayersCount();
        }
        return 0;
    }
}; 

std::unique_ptr<MatchMakingEngine> MatchMakingEngine::instance = nullptr;
std::mutex MatchMakingEngine::mutex;