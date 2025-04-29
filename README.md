# Gacha Game Simulator (C++)

A C++ implementation of a gacha game system that simulates the mechanics found in popular gacha/loot box games. This project demonstrates probability-based rewards, currency management, and inventory systems using object-oriented programming principles.

## Demonstration
[![Watch Demo](https://img.shields.io/badge/Watch%20Demo-%F0%9F%8E%A5-blue?style=for-the-badge)](https://drive.google.com/file/d/1DvCYzjW76vWijHl1x3YyQ1lDX_cx-DyU/view?usp=sharing)


## How Gacha Games Work

Gacha games are a genre of games that feature random reward mechanics similar to loot boxes or trading card packs. Key elements include:

- **Probability Distribution**: Items have different rarity tiers (Common, Uncommon, Rare, Epic, Legendary, Mythical) with weighted drop chances
- **In-Game Currency**: Players use earned or purchased currency to make pulls
- **Pity Systems**: Guaranteed high-rarity drops after certain numbers of attempts
- **Collection Tracking**: Systems to manage and display obtained items

## Project Structure

This C++ implementation features:

### 1. Core Gacha Mechanics
- Random number generation for item distribution
- Configurable drop rates for different rarity tiers
- Pull simulation with guaranteed results display

### 2. Player Systems
- Currency tracking (premium and standard currencies)
- Inventory management

### 3. Data Management
- Item database with rarity classifications
- Player save/load functionality
- Statistical tracking of pull results

## Building and Running

### Requirements
- C++11 compatible compiler (GCC)
- CMake (optional, for build configuration)

### Usage
1. Run the compiled executable
2. Follow on-screen menu options to:
    - Perform gacha pulls
    - View inventory
    - Check currency balances
    - Sell items
    - Exit

## Configuration Options

Developers can modify:
- `GachaGame.h` - contains the entire system of the Gacha Game
## Future Enhancements

Potential improvements include:
- Multiplayer trading systems
- Event-based limited-time banners
- Cloud save functionality

## How to Run

- Make sure you have `raylib` installed (`brew install raylib` recommended)
- Open terminal and run `./build/GachaGame`
