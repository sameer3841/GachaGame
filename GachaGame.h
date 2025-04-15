#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

class GachaItem final {
public:
    GachaItem(string name, int rarity) : name(name), rarity(rarity) {}
    ~GachaItem() = default;

    [[nodiscard]] string getName() const { return name; }
    [[nodiscard]] int getRarity() const { return rarity; }

private:
    string name;
    int rarity;
};

class GachaPool {
public:
    void addItem(const shared_ptr<GachaItem>& item, double rate) {
        items.push_back(item);
        rates.push_back(rate);
        totalRate += rate;
    }

    shared_ptr<GachaItem> pull() {
        uniform_real_distribution distribution(0.0, totalRate);
        const double randomValue = distribution(generator);
        double cumulative = 0.0;

        for (size_t i = 0; i < items.size(); ++i) {
            cumulative += rates[i];
            if (randomValue <= cumulative) return items[i];
        }
        return nullptr; // Should not happen, should always return an item by index
    }

    vector<shared_ptr<GachaItem>> getItems() { return items; }

    void increaseRate(size_t index, double increaseBy) {
        rates[index] += increaseBy;
        totalRate += increaseBy;
    }

private:
    vector<shared_ptr<GachaItem>> items;
    vector<double> rates;
    double totalRate = 0.0;
    default_random_engine generator{random_device{}()};
};



class Player {
public:
    explicit Player(string name) : name(name), currency(100) {}

    void addItem(const shared_ptr<GachaItem>& item) {
        inventory.push_back(item);
        string s = "★";
        int i = item->getRarity();

        cout << "\nObtained: " << item->getName() << " [" << i << "★]" << endl;
    }

    void showInventory() const {
        cout << "\n--- Inventory ---\n";
        for (int item = 0; item < inventory.size(); ++item) {
            cout << item+1 << ". " << inventory[item]->getName() << " [" << inventory[item]->getRarity() << "★]" << endl;
        }
    }

    [[nodiscard]] bool inventoryIsFull() const { return inventory.size() >= 15; }
    [[nodiscard]] bool canPull(int cost) const { return currency >= cost; }

    void spendCurrency(int amount) { currency -= amount; }
    void earnCurrency(int amount) { currency += amount; }
    int getCurrency() const { return currency; }

    void sellItem(int index) {
        if (index < 1 || index > inventory.size()) {
            cout << "Invalid item selection!" << endl;
            return;
        }

        shared_ptr<GachaItem> item = inventory[index - 1];
        const int sellValue = getSellValue(item->getRarity());

        inventory.erase(inventory.begin() + (index - 1));
        currency += sellValue;

        cout << "Sold: " << item->getName() << " for " << sellValue << " currency." << endl;
    }

private:
    string name;
    int currency;
    vector<shared_ptr<GachaItem>> inventory;

    static int getSellValue(int rarity) {
        switch (rarity) {
            case 1: return 5;   // Common
            case 2: return 10;  // Uncommon
            case 3: return 20;  // Rare
            case 4: return 50;  // Epic
            case 5: return 100; // Legendary
            default: return 0;
        }
    }
};

class GachaGame {
public:
    void run() {
        setupPool();
        mainMenu();
    }

private:
    GachaPool pool;
    Player player{"Player"};
    int pityCounter = 0;  // Counter for unsuccessful pulls

    void setupPool() {
        // Common Weapons
        pool.addItem(make_shared<GachaItem>("Common Sword", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Rusty Dagger", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Wooden Ladle", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Tree Branch", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Small Rock", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Wooden Club", 1), 60.0);
        pool.addItem(make_shared<GachaItem>("Common Spear", 1), 60.0);

        // Uncommon Weapons
        pool.addItem(make_shared<GachaItem>("Torch", 2), 45.0);
        pool.addItem(make_shared<GachaItem>("Kitchen Knife", 2), 45.0);
        pool.addItem(make_shared<GachaItem>("Skeleton Arm", 2), 45.0);
        pool.addItem(make_shared<GachaItem>("Reinforced Sword", 2), 45.0);
        pool.addItem(make_shared<GachaItem>("Reinforced Spear", 2), 45.0);

        // Rare Weapons
        pool.addItem(make_shared<GachaItem>("Rare Spear", 3), 30.0);
        pool.addItem(make_shared<GachaItem>("Fire Sword", 3), 30.0);
        pool.addItem(make_shared<GachaItem>("Ice Sword", 3), 30.0);
        pool.addItem(make_shared<GachaItem>("Rare Claymore", 3), 30.0);

        // Epic Weapons
        pool.addItem(make_shared<GachaItem>("Epic Staff", 4), 9.0);
        pool.addItem(make_shared<GachaItem>("Fire Claymore", 4), 9.0);
        pool.addItem(make_shared<GachaItem>("Ice Claymore", 4), 9.0);

        // Legendary Weapons
        pool.addItem(make_shared<GachaItem>("Legendary Blade", 5), 1.0);
        pool.addItem(make_shared<GachaItem>("Sword of Sparda", 5), 1.0);
    }

    void mainMenu() {
        int choice;
        do {
            cout << "\n1. Pull (Cost: 10)\n2. Show Inventory\n3. Show Currency\n4. Sell Item\n0. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    pullGacha();
                    break;

                case 2:
                    player.showInventory();
                    break;

                case 3:
                    cout << "\nCurrency: " << player.getCurrency() << endl;
                    break;

                case 4: {
                    player.showInventory();
                    cout << "\nSelect an item to sell (Choose by slot number): ";
                    int itemNumber;
                    cin >> itemNumber;
                    player.sellItem(itemNumber);
                    break;
                }

                case 0:
                    cout << "\nGoodbye! 👋" << endl;
                    break;

                default:
                    cout << "\nInvalid choice!" << endl;
                    break;
            }
        } while (choice != 0);
    }

    void pullGacha() {
        const int cost = 10;
        if (player.inventoryIsFull()) {cout << "Please sell to make space!" << endl; return;}
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
        } else cout << "You cannot afford anymore. ☹️" << endl;

    }

    void increaseHighRarityOdds() {
        cout << "\nPity system activated, odds increased!" << endl;

        for (size_t i = 0; i < pool.getItems().size(); ++i) {
            auto item = pool.getItems()[i];
            if (item->getRarity() >= 4) pool.increaseRate(i, 50.0);
        }
    }
};