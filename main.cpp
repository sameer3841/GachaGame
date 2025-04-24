#define RAYLIB_CLITERAL_SUPPORT
#include "raylib.h"
#include "GachaGame.h"
#include <string>

Color GetRarityColor(int rarity) {
    switch (rarity) {
        case 1: return LIGHTGRAY;                                 // Common
        case 2: return GREEN;                                     // Uncommon
        case 3: return SKYBLUE;                                   // Rare
        case 4: return (Color){180, 120, 255, 255};  // Epic
        case 5: return (Color){255, 215, 0, 255};    // Legendary
        case 6: return (Color){255, 80, 80, 255};    // Mythical
        default: return WHITE;
    }
}

std::string operator*(const std::string& str, int times) {
    std::string result;
    result.reserve(str.size() * times);
    for (int i = 0; i < times; ++i) result += str;
    return result;
}

int main() {
    srand(time(NULL));
    const int screenWidth = 900, screenHeight = 600, inventoryWidth = 300;

    InitWindow(screenWidth, screenHeight, "Gacha Game");
    SetTargetFPS(60);

    GachaGame game;
    game.setupPool();
    Player& player = game.getPlayer();

    std::string lastMessage = "Press [SPACE] to pull!";
    int lastPulledRarity = 1;

    Color bgColor = (Color){30, 30, 30, 255}, primaryText = RAYWHITE, secondaryText = (Color){180, 180, 180, 255};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bgColor);

        // Game Panel (Left side)
        DrawText("Gacha Game", 20, 20, 32, primaryText);
        DrawText(TextFormat("Currency: %d", player.getCurrency()), 20, 70, 22, secondaryText);
        DrawText("Press [SPACE] to Pull", 20, 100, 18, secondaryText);
        DrawText(lastMessage.c_str(), 20, 140, 22, GetRarityColor(lastPulledRarity));

        // Inventory Panel (Right side)
        DrawRectangle(screenWidth - inventoryWidth, 0, inventoryWidth, screenHeight, (Color){25, 25, 25, 255});
        DrawText("--- Inventory (click to sell) ---", screenWidth - inventoryWidth + 10, 20, 20, primaryText);

        const auto& inv = player.getInventory();
        int y = 50;
        for (size_t i = 0; i < inv.size() && y < screenHeight - 30; ++i, y += 24) {
            std::string line = inv[i]->getName() + " [" + std::to_string(inv[i]->getRarity()) + "*]";
            Color rarityColor = GetRarityColor(inv[i]->getRarity());
            int x = screenWidth - inventoryWidth + 10;

            DrawText(line.c_str(), x, y, 18, rarityColor);

            Rectangle itemBox = {(float)x, (float)y, (float)(inventoryWidth - 20), 20};
            if (CheckCollisionPointRec(GetMousePosition(), itemBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                player.sellItem(i + 1);
                lastMessage = "Sold: " + inv[i]->getName();
                break;
            }
        }

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (GetMouseX() >= screenWidth - inventoryWidth) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);


        // Pull logic
        if (IsKeyPressed(KEY_SPACE)) {
            auto item = game.pullGacha();
            if (item) {
                lastPulledRarity = item->getRarity();
                std::string s = "*";
                lastMessage = "Pulled: " + item->getName() + " [" + (std::to_string(lastPulledRarity) + s*lastPulledRarity) + "️]";
            } else lastMessage = "Cannot pull! Not enough currency or inventory full.";
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
