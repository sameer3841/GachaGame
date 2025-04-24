#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <map>

class GachaItem final {
public:
    GachaItem(const std::string& name, int rarity) : name(name), rarity(rarity) {}
    ~GachaItem() = default;

    std::string getName() const { return name; }
    int getRarity() const { return rarity; }

private:
    std::string name;
    int rarity;
};

class GachaPool {
public:
    GachaPool() {
        std::random_device rd;
        generator.seed(rd());
        totalRate = 0.0;
    }

    void addItem(const std::shared_ptr<GachaItem>& item, double rate) {
        items.push_back(item);
        rates.push_back(rate);
        totalRate += rate;
    }

    std::shared_ptr<GachaItem> pull() {
        std::uniform_real_distribution<double> distribution(0.0, totalRate);
        double randomValue = distribution(generator);
        double cumulative = 0.0;

        for (size_t i = 0; i < items.size(); ++i) {
            cumulative += rates[i];
            if (randomValue <= cumulative) return items[i];
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<GachaItem>> getItems() { return items; }

    void increaseRate(size_t index, double increaseBy) {
        rates[index] += increaseBy;
        totalRate += increaseBy;
    }

    void decreaseRate(size_t index, double decreaseBy) {
        rates[index] -= decreaseBy;
        totalRate -= decreaseBy;
    }

private:
    std::vector<std::shared_ptr<GachaItem>> items;
    std::vector<double> rates;
    double totalRate;
    std::default_random_engine generator;
};

class Player {
public:
    Player(const std::string& name) : name(name), currency(100) {}

    void addItem(const std::shared_ptr<GachaItem>& item) {
        inventory.push_back(item);
        std::cout << "\nObtained: " << item->getName()
                  << " [" << item->getRarity() << "*]" << std::endl;
    }

    void showInventory() const {
        std::cout << "\n--- Inventory ---\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i]->getName()
                      << " [" << inventory[i]->getRarity() << "*]" << std::endl;
        }
    }

    const std::vector<std::shared_ptr<GachaItem>>& getInventory() const {
        return inventory;
    }

    bool inventoryIsFull() const { return inventory.size() >= 15; }
    bool canPull(int cost) const { return currency >= cost; }

    void spendCurrency(int amount) { currency -= amount; }
    void earnCurrency(int amount) { currency += amount; }
    int getCurrency() const { return currency; }

    void sellItem(int index) {
        if (index < 1 || index > static_cast<int>(inventory.size())) {
            std::cout << "Invalid item selection!" << std::endl;
            return;
        }
        auto item = inventory[index - 1];
        int sellValue = getSellValue(item->getRarity());
        inventory.erase(inventory.begin() + index - 1);
        currency += sellValue;
        std::cout << "Sold: " << item->getName() << " for " << sellValue << " currency.\n";
    }

private:
    std::string name;
    int currency;
    std::vector<std::shared_ptr<GachaItem>> inventory;

    static int getSellValue(int rarity) {
        switch (rarity) {
            case 1: return 5;
            case 2: return 10;
            case 3: return 20;
            case 4: return 50;
            case 5: return 100;
            case 6: return 150;
            default: return 0;
        }
    }
};

class GachaGame {
public:
    GachaGame() : player("Player"), pityCounter(0) {}

    void run() {
        setupPool();

        int choice;
        do {
            std::cout << "\n=== Gacha Game Menu ===\n";
            std::cout << "1. Pull (Cost: 10)\n";
            std::cout << "2. Show Inventory\n";
            std::cout << "3. Show Currency\n";
            std::cout << "4. Sell Item\n";
            std::cout << "0. Exit\n";
            std::cout << "Choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    auto item = pullGacha();
                    if (item) {
                        std::cout << "Pulled: " << item->getName()
                                  << " [" << item->getRarity() << "]\n";
                    }
                    break;
                }
                case 2:
                    player.showInventory();
                    break;
                case 3:
                    std::cout << "\nCurrency: " << player.getCurrency() << "\n";
                    break;
                case 4: {
                    int itemNum;
                    do {
                        player.showInventory();
                        std::cout << "Select item number to sell (0 to exit): ";
                        std::cin >> itemNum;
                        if (itemNum != 0) player.sellItem(itemNum);
                    } while (itemNum != 0);
                    break;
                }
                case 0:
                    std::cout << "\nGoodbye!\n";
                    break;
                default:
                    std::cout << "\nInvalid choice.\n";
                    break;
            }
        } while (choice != 0);
    }

    void setupPool() {
        // Rarity totals
        std::map<int, double> rarityProb;
        rarityProb[1] = 80.0;
        rarityProb[2] = 25.0;
        rarityProb[3] = 15.0;
        rarityProb[4] = 7.0;
        rarityProb[5] = 1.9;
        rarityProb[6] = 0.01;

        // Items per rarity
        std::map<int, std::vector<std::string> > items;
        items[1].push_back("Common Sword");
        items[1].push_back("Rusty Dagger");
        items[1].push_back("Wooden Ladle");
        items[1].push_back("Tree Branch");
        items[1].push_back("Small Rock");
        items[1].push_back("Wooden Club");
        items[1].push_back("Common Spear");

        items[2].push_back("Torch");
        items[2].push_back("Kitchen Knife");
        items[2].push_back("Skeleton Arm");
        items[2].push_back("Reinforced Sword");
        items[2].push_back("Reinforced Spear");

        items[3].push_back("Rare Spear");
        items[3].push_back("Fire Sword");
        items[3].push_back("Ice Sword");
        items[3].push_back("Rare Claymore");

        items[4].push_back("Epic Staff");
        items[4].push_back("Fire Claymore");
        items[4].push_back("Ice Claymore");

        items[5].push_back("Legendary Blade");
        items[5].push_back("Sword of Sparda");

        items[6].push_back("Master Sword");

        // Distribute rates
        for (std::map<int, std::vector<std::string> >::iterator it = items.begin(); it != items.end(); ++it) {
            int rarity = it->first;
            std::vector<std::string>& names = it->second;
            double ratePerItem = rarityProb[rarity] / names.size();
            for (size_t i = 0; i < names.size(); ++i) {
                pool.addItem(std::make_shared<GachaItem>(names[i], rarity), ratePerItem);
            }
        }
    }

    Player& getPlayer() { return player; }

    std::shared_ptr<GachaItem> pullGacha() {
        const int cost = 10;
        if (player.inventoryIsFull()) {
            std::cout << "Please sell to make space!" << std::endl;
            return nullptr;
        }

        if (player.canPull(cost)) {
            auto item = pool.pull();
            player.addItem(item);
            player.spendCurrency(cost);

            if (pityCounter >= 5) {
                pityCounter = 0;
                decreaseHighRarityOdds();
            }
            else if (item->getRarity() >= 3) pityCounter = 0;
            else pityCounter++;

            if (pityCounter >= 5) increaseHighRarityOdds();

            return item;
        }
        std::cout << "You cannot afford anymore. ☹️" << std::endl;
        return nullptr;
    }

private:
    GachaPool pool;
    Player player;
    int pityCounter;

    void increaseHighRarityOdds() {
        std::cout << "\nPity system activated, odds increased!" << std::endl;
        for (size_t i = 0; i < pool.getItems().size(); ++i) {
            auto item = pool.getItems()[i];
            if (item->getRarity() == 4) pool.increaseRate(i, 50.0);
            if (item->getRarity() == 5) pool.increaseRate(i, 20.0);
            if (item->getRarity() == 6) pool.increaseRate(i, 10.0);
        }
    }

    void decreaseHighRarityOdds() {
        std::cout << "\nPity system deactivated!" << std::endl;
        for (size_t i = 0; i < pool.getItems().size(); ++i) {
            auto item = pool.getItems()[i];
            if (item->getRarity() == 4) pool.decreaseRate(i, 50.0);
            if (item->getRarity() == 5) pool.decreaseRate(i, 20.0);
            if (item->getRarity() == 6) pool.decreaseRate(i, 10.0);
        }
    }
};
