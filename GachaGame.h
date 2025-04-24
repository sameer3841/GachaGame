#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>

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

    std::vector<std::shared_ptr<GachaItem> > getItems() { return items; }

    void increaseRate(size_t index, double increaseBy) {
        rates[index] += increaseBy;
        totalRate += increaseBy;
    }

private:
    std::vector<std::shared_ptr<GachaItem> > items;
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
                  << " [" << item->getRarity() << "\u2605]" << std::endl;
    }

    void showInventory() const {
        std::cout << "\n--- Inventory ---\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i]->getName()
                      << " [" << inventory[i]->getRarity() << "\u2605]" << std::endl;
        }
    }

    const std::vector<std::shared_ptr<GachaItem> >& getInventory() const {
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
    std::vector<std::shared_ptr<GachaItem> > inventory;

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
        pool.addItem(std::make_shared<GachaItem>("Common Sword", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Rusty Dagger", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Wooden Ladle", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Tree Branch", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Small Rock", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Wooden Club", 1), 60.0);
        pool.addItem(std::make_shared<GachaItem>("Common Spear", 1), 60.0);

        pool.addItem(std::make_shared<GachaItem>("Torch", 2), 45.0);
        pool.addItem(std::make_shared<GachaItem>("Kitchen Knife", 2), 45.0);
        pool.addItem(std::make_shared<GachaItem>("Skeleton Arm", 2), 45.0);
        pool.addItem(std::make_shared<GachaItem>("Reinforced Sword", 2), 45.0);
        pool.addItem(std::make_shared<GachaItem>("Reinforced Spear", 2), 45.0);

        pool.addItem(std::make_shared<GachaItem>("Rare Spear", 3), 30.0);
        pool.addItem(std::make_shared<GachaItem>("Fire Sword", 3), 30.0);
        pool.addItem(std::make_shared<GachaItem>("Ice Sword", 3), 30.0);
        pool.addItem(std::make_shared<GachaItem>("Rare Claymore", 3), 30.0);

        pool.addItem(std::make_shared<GachaItem>("Epic Staff", 4), 9.0);
        pool.addItem(std::make_shared<GachaItem>("Fire Claymore", 4), 9.0);
        pool.addItem(std::make_shared<GachaItem>("Ice Claymore", 4), 9.0);

        pool.addItem(std::make_shared<GachaItem>("Legendary Blade", 5), 1.0);
        pool.addItem(std::make_shared<GachaItem>("Sword of Sparda", 5), 1.0);

        pool.addItem(std::make_shared<GachaItem>("Master Sword", 6), 0.05);
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

            if (item->getRarity() >= 4) pityCounter = 0;
            else pityCounter++;

            if (pityCounter >= 5) {
                increaseHighRarityOdds();
                pityCounter = 0;
            }

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
            if (item->getRarity() >= 4) pool.increaseRate(i, 50.0);
        }
    }
};
