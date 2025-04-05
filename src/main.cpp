#include <iostream>
#include <chrono>
#include <thread>
#include "MatchMakingEngine.hpp"

void printTest(const std::string& testName) {
    std::cout << "\n=== " << testName << " ===" << std::endl;
}

int main() {
    auto& engine = MatchMakingEngine::getInstance();

    // Test 1: Regular TwoVTwo match with Gold players (SameRank matching)
    printTest("Test 1: Regular TwoVTwo match with Gold players");
    for (int i = 1; i <= 4; ++i) {
        auto player = std::make_shared<Player>("Gold" + std::to_string(i), Rank::Gold, MatchingType::SameRank);
        engine.addPlayer(player, GameMode::TwoVTwo, GameLocation::CastleTown);
    }

    // Test 2: AnyRank matching type players forming their own FastDraw match
    printTest("Test 2: Gold players with AnyRank matching type forming their own FastDraw match");
    for (int i = 1; i <= 2; ++i) {
        auto player = std::make_shared<Player>("Gold_AnyRank" + std::to_string(i), Rank::Gold, MatchingType::AnyRank);
        engine.addPlayer(player, GameMode::FastDraw, GameLocation::AirBase);
    }

    // Test 3: Mix of Platinum and Gold players with different matching types in Raid
    printTest("Test 3: Mix of Platinum and Gold players with different matching types in Raid");
    // Add 2 Platinum players and 1 Gold player
    for (int i = 1; i <= 2; ++i) {
        auto player = std::make_shared<Player>("Platinum" + std::to_string(i), Rank::Platinum, MatchingType::SameRank);
        engine.addPlayer(player, GameMode::Raid, GameLocation::SavageLand);
    }
    // Add the Gold player
    auto goldPlayer = std::make_shared<Player>("Gold_Player", Rank::Gold, MatchingType::SameRank);
    engine.addPlayer(goldPlayer, GameMode::Raid, GameLocation::SavageLand);

    // Add 3 AnyRank Platinum players to try to complete the Raid team
    for (int i = 1; i <= 3; ++i) {
        auto player = std::make_shared<Player>("Platinum_AnyRank" + std::to_string(i), Rank::Platinum, MatchingType::AnyRank);
        engine.addPlayer(player, GameMode::Raid, GameLocation::SavageLand);
    }

    // Test 4: AnyRank matching type players joining SameRank match
    printTest("Test 4: Bronze players with different matching types");
    // First add 2 AnyRank Bronze players
    for (int i = 1; i <= 2; ++i) {
        auto player = std::make_shared<Player>("Bronze_AnyRank" + std::to_string(i), Rank::Bronze, MatchingType::AnyRank);
        engine.addPlayer(player, GameMode::TwoVTwo, GameLocation::AirBase);
    }
    // Then add 2 SameRank Bronze players to complete the match
    for (int i = 1; i <= 2; ++i) {
        auto player = std::make_shared<Player>("Bronze" + std::to_string(i), Rank::Bronze, MatchingType::SameRank);
        engine.addPlayer(player, GameMode::TwoVTwo, GameLocation::AirBase);
    }

    return 0;
}