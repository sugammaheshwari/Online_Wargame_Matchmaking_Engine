# Online Wargame Matchmaking Engine

A sophisticated matchmaking engine designed for online wargames, supporting various game modes, player ranks, and matching types.

## Features

- **Multiple Game Modes**: Supports TwoVTwo, FastDraw, and Raid game modes
- **Location-based Matching**: Players can be matched based on different game locations
- **Rank-based Matchmaking**: Supports multiple player ranks (Bronze, Silver, Gold, Platinum)
- **Flexible Matching Types**: 
  - SameRank: Matches players of the same rank
  - AnyRank: Flexible matching across ranks when appropriate
- **Thread-safe Implementation**: Uses singleton pattern with mutex for thread safety
- **Factory Pattern**: Implements factory pattern for game bucket creation

## Project Structure

- `MatchMakingEngine.hpp`: Core matchmaking engine implementation
- `GameBucket.hpp`: Game bucket management
- `BucketFactory.hpp`: Factory pattern implementation for bucket creation
- `Player.hpp`: Player class implementation
- `Enums.hpp`: Game-related enumerations
- `main.cpp`: Test cases and demo implementation

## Building the Project

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make

# Run the demo
./matchmaking_demo
```

## Test Cases

1. Regular TwoVTwo match with Gold players (SameRank matching)
2. Gold players with AnyRank matching type forming their own FastDraw match
3. Mix of Platinum and Gold players with different matching types in Raid
4. Bronze players with different matching types in TwoVTwo

## Binary

The compiled binary is available in the `bin` directory for backup purposes.

## License

MIT License