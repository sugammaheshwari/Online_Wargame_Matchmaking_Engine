#pragma once
#include <memory>
#include "GameBucket.hpp"

class BucketFactory {
public:
    static std::unique_ptr<GameBucket> createBucket(GameMode mode, GameLocation location, Rank rank, MatchingType type) {
        if (type == MatchingType::AnyRank) {
            return createAnyRankBucket(mode, location);
        } else {
            return createSameRankBucket(mode, location, rank);
        }
    }

private:
    static std::unique_ptr<GameBucket> createAnyRankBucket(GameMode mode, GameLocation location) {
        return std::make_unique<GameBucket>(mode, location, MatchingType::AnyRank);
    }

    static std::unique_ptr<GameBucket> createSameRankBucket(GameMode mode, GameLocation location, Rank rank) {
        return std::make_unique<GameBucket>(mode, location, rank, MatchingType::SameRank);
    }
};