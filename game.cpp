#include "game.h"
#include "player.h"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <stdlib.h>
#include "enemy.h"

Pilgrimage::Pilgrimage() :
    actions_left_(GameConfig::GetInstance().GetInt("day_duration", 3)),
    is_day_(true),
    enemy_types_(),
    item_types_()
{
    std::string enemyTypes = GameConfig::GetInstance().GetString("enemy_types", "wolf,bandit");
    size_t pos_enemy = 0;
    while ((pos_enemy = enemyTypes.find(',')) != std::string::npos) {
        enemy_types_.push_back(enemyTypes.substr(0, pos_enemy));
        enemyTypes.erase(0, pos_enemy + 1);
    }
    if (!enemyTypes.empty()) {
        enemy_types_.push_back(enemyTypes);
    }

    std::string itemTypes = GameConfig::GetInstance().GetString("item_types", "healthpotion,ironsword");
    size_t pos_item = 0;
    while ((pos_item = itemTypes.find(',')) != std::string::npos) {
        item_types_.push_back(itemTypes.substr(0, pos_item));
        itemTypes.erase(0, pos_item + 1);
    }
    if (!itemTypes.empty()) {
        item_types_.push_back(itemTypes);
    }

    std::srand(std::time(nullptr));
}

void Pilgrimage::PrintStatus() const {
    std::cout << "\n--- Day " << player_.day() << " (" << player_.days_left()
        << " days left) ---\n";
    std::cout << "HP: " << player_.health() << "/" << GameConfig::GetInstance().GetInt("max_health", 100) << "\n";
    std::cout << "Stamina: " << player_.stamina() << "/" << GameConfig::GetInstance().GetInt("max_stamina", 100) << "\n";
    std::cout << "Gold: " << player_.gold() << "\n";
    std::cout << "Wagon HP: " << player_.wagon_health() << "/"
        << GameConfig::GetInstance().GetInt("wagon_health_max", 100) << "\n";
    std::cout << "Distance traveled: " << player_.distance_covered() << " / "
        << GameConfig::GetInstance().GetInt("distance_to_temple", 1000) << " km\n";

    if (is_day_) {
        std::cout << "Day actions left: " << actions_left_ << "\n";
    }
    else {
        std::cout << "Night time. Time to rest, defend or keep walking!\n";
    }

    std::cout << "------------------------\n";
}

void Pilgrimage::HandleDay() {
    while (actions_left_ > 0 && player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        PrintStatus();

        std::cout << "Day actions (" << actions_left_ << " left):\n";
        std::cout << "1. Explore area (-"
            << GameConfig::GetInstance().GetInt("explore_stamina_cost", 20)
            << " stamina)\n";
        std::cout << "2. Repair wagon (-"
            << GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20)
            << " gold)\n";
        std::cout << "3. Rest (+"
            << GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40)
            << " stamina)\n";
        std::cout << "4. Continue on the trail (+"
            << GameConfig::GetInstance().GetInt("daily_progress", 80)
            << " km, -" << GameConfig::GetInstance().GetInt("daily_progress_cost", 30)
            << " stamina)\n";
        std::cout << "5. Check inventory.\n";

        int choice = GetPlayerChoice(1, 5);

        system("cls");

        switch (choice) {
        case 1: Explore(); break;
        case 2: RepairWagon(); break;
        case 3: Rest(); break;
        case 4: DailyProgress(); break;
        case 5: HandleInventory(); break;
        }

        if (!player_.IsAlive() || !player_.HasWagon() || player_.HasReachedTemple()) {
            break;
        }
    }

    if (actions_left_ <= 0) {
        is_day_ = false;
        actions_left_ = 1;
    }
}

void Pilgrimage::HandleNight() {
    while (actions_left_ > 0 && player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        PrintStatus();

        std::cout << "Night actions:\n";
        std::cout << "1. Sleep deeply (+"
            << GameConfig::GetInstance().GetInt("rest_health_gain", 30)
            << " HP, risk of attack)\n";
        std::cout << "2. Stand guard (-"
            << GameConfig::GetInstance().GetInt("defend_stamina_cost", 30)
            << " stamina, protects wagon)\n";
        std::cout << "3. Night march (+"
            << GameConfig::GetInstance().GetInt("forced_march_bonus", 30)
            << " km, -" << GameConfig::GetInstance().GetInt("forced_march_cost", 40)
            << " stamina, -" << GameConfig::GetInstance().GetInt("forced_march_health_cost", 10)
            << " HP)\n";

        int choice = GetPlayerChoice(1, 3);

        switch (choice) {
        case 1: DeepSleep(); break;
        case 2: DefendWagon(); break;
        case 3: NightMarch(); break;
        }

        if (!player_.IsAlive() || !player_.HasWagon() || player_.HasReachedTemple()) {
            break;
        }
    }

    if (actions_left_ <= 0) {
        player_.NextDay();
        is_day_ = true;
        actions_left_ = GameConfig::GetInstance().GetInt("day_duration", 3);
    }
}

void Pilgrimage::Run() {
    PrintWelcomeMessage();

    while (player_.IsAlive() && player_.HasWagon() && player_.HasTime() && !player_.HasReachedTemple()) {
        if (is_day_) {
            HandleDay();
        }
        else {
            HandleNight();
        }
    }

    PrintGameOver();
}

void Pilgrimage::PrintWelcomeMessage() const {
    std::cout << "Welcome to 'Pilgrimage'!\n";
    std::cout << "You need to travel " << player_.distance_left() << " km in "
        << GameConfig::GetInstance().GetInt("max_days", 14) << " days.\n\n";
}

void Pilgrimage::Explore() {
    if (player_.stamina() < 20) {
        std::cout << "\nToo tired to explore.\n";
        return;
    }

    player_.SpendStamina(20);
    actions_left_--;

    int event = rand() % 6;
    Item new_item = GenerateRandomItem();

    switch (event) {
    case 0:
        player_.AddGold(10 + rand() % 20);
        std::cout << "\nYou found traveler donations! Gold +" << player_.gold() << ".\n";
        break;
    case 1:
        std::cout << "\nFound " << new_item.GetName() << ". Added to your inventory!\n";
        AddItemToPlayer(new_item.GetName());
        break;
    case 2:
        player_.DamageWagon(5);
        std::cout << "\nEncountered rough terrain. Wagon damaged.\n";
        break;
    case 3:
        player_.RestoreStamina(15);
        std::cout << "\nFound a peaceful spot. Stamina +15.\n";
        break;
    case 4:
        std::cout << "\nYou're under attack!\n";
        CombatEncounter();
        break;
    case 5:
        std::cout << "\nLong exploration yielded nothing special.\n";
        break;
    }
}

void Pilgrimage::RepairWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20);
    if (player_.gold() < cost) {
        std::cout << "You need " << cost << " gold!\n";
        return;
    }

    player_.SpendGold(cost);
    player_.RepairWagon(GameConfig::GetInstance().GetInt("wagon_repair_amount", 30));
    actions_left_--;
    std::cout << "Wagon repaired!\n";
}

void Pilgrimage::Rest() {
    system("cls");
    int gain = GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40);
    player_.RestoreStamina(gain);
    actions_left_--;
    std::cout << "Rested! +" << gain << " stamina\n";
}

void Pilgrimage::DailyProgress() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("daily_progress_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "You need " << cost << " stamina!\n";
        return;
    }

    player_.SpendStamina(cost);
    player_.AddDistance(GameConfig::GetInstance().GetInt("daily_progress", 80));
    actions_left_--;
    std::cout << "Traveled +" << GameConfig::GetInstance().GetInt("daily_progress", 80) << " km!\n";

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}

void Pilgrimage::DeepSleep() {
    system("cls");
    player_.Heal(GameConfig::GetInstance().GetInt("rest_health_gain", 30));
    player_.RestoreStamina(GameConfig::GetInstance().GetInt("max_stamina", 100));
    std::cout << "Well rested! Fully recovered\n";
    actions_left_--;

    if (rand() % 2 == 0) {
        int damage = GameConfig::GetInstance().GetInt("base_monster_damage", 15) + (rand() % 20);
        player_.DamageWagon(damage);
        std::cout << "Monsters attacked! Wagon -" << damage << " durability\n";
    }
}

void Pilgrimage::DefendWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("defend_stamina_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "You need " << cost << " stamina!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;

    if (rand() % 10 < 7) {
        CombatEncounter();
    }
    else {
        std::cout << "\nNo one attacked you tonight.\n";
    }
}

void Pilgrimage::NightMarch() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("forced_march_cost", 40);
    if (player_.stamina() < cost) {
        std::cout << "You need " << cost << " stamina!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;
    player_.TakeDamage(GameConfig::GetInstance().GetInt("forced_march_health_cost", 10));
    player_.AddDistance(GameConfig::GetInstance().GetInt("forced_march_bonus", 30));
    std::cout << "Night march! +" << GameConfig::GetInstance().GetInt("forced_march_bonus", 30) << " km\n";

    if (rand() % 3 == 0) {
        int damage = 10 + (rand() % 15);
        player_.DamageWagon(damage);
        std::cout << "Wagon damaged: -" << damage << " durability\n";
    }

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}

Enemy Pilgrimage::GenerateRandomEnemy() const {
    if (enemy_types_.empty()) return Enemy("wolf");
    int index = rand() % enemy_types_.size();
    return Enemy(enemy_types_[index]);
}

void Pilgrimage::CombatEncounter() {
    Enemy enemy = GenerateRandomEnemy();
    std::cout << "You encountered a " << enemy.GetName() << "!\n";

    while (enemy.IsAlive() && player_.IsAlive()) {
        system("cls");
        std::cout << enemy.GetName() << ": " << enemy.GetHealth() << " HP/"
            << enemy.GetDamage() << " damage\n";
        std::cout << "1. Attack\n2. Try to escape\n";

        int choice = GetPlayerChoice(1, 2);

        if (choice == 1) {
            int playerDamage = GameConfig::GetInstance().GetInt("player_base_damage", 15);
            enemy.TakeDamage(playerDamage);
            std::cout << "You dealt " << playerDamage << " damage!\n";

            if (enemy.IsAlive()) {
                player_.TakeDamage(enemy.GetDamage());
                std::cout << enemy.GetName() << " counterattacks! -"
                    << enemy.GetDamage() << " HP\n";
                std::cout << "Your HP: " << player_.health() << "/" << GameConfig::GetInstance().GetInt("max_health", 100) << "\n";
                std::cout << "Enter 0 to continue!\n";
                int newchoice = GetPlayerChoice(0, 0);
            }
        }
        else {
            if (rand() % 2 == 0) {
                system("cls");
                std::cout << "You managed to escape!\n";
                return;
            }
            std::cout << "Escape failed! " << enemy.GetName() << " attacks!\n";
            player_.TakeDamage(enemy.GetDamage());
            std::cout << "Your HP: " << player_.health() << "/" << GameConfig::GetInstance().GetInt("max_health", 100) << "\n";
            std::cout << "Enter 0 to continue!\n";
            int newchoice = GetPlayerChoice(0, 0);
        }
    }

    if (!enemy.IsAlive()) {
        system("cls");
        int reward = enemy.GetGoldReward();
        player_.AddGold(reward);
        std::cout << "You defeated the " << enemy.GetName() << " and got "
            << reward << " gold!\n";
    }
}

void Pilgrimage::RandomTravelEvent() {
    system("cls");
    std::cout << "\n--- On the road ---\n";
    switch (rand() % 4) {
    case 0:
        player_.AddDistance(15);
        std::cout << "Found a shortcut! +15 km\n";
        break;
    case 1:
        player_.DamageWagon(10);
        std::cout << "Difficult road! Wagon -10HP\n";
        break;
    case 2:
        player_.Heal(15);
        std::cout << "Helped a traveler! +15 HP\n";
        break;
    case 3:
        std::cout << "You're under attack!\n";
        CombatEncounter();
        break;
    }
    std::cout << "---------------\n";
}

void Pilgrimage::HandleInventory() {
    const auto& inventory = player_.GetInventory();

    if (inventory.empty()) {
        std::cout << "Inventory is empty.\n";
        return;
    }

    std::cout << "===== INVENTORY =====\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Item& item = inventory[i];
        std::cout << i + 1 << ". " << item.GetName()
            << " - " << item.GetDescription() << "\n";
    }
}


Item Pilgrimage::GenerateRandomItem() const {
    if (item_types_.empty()) return Item("health_potion");
    int index = rand() % item_types_.size();
    return Item(item_types_[index]);
}

Item Pilgrimage::GetItem(const std::string& itemId) const {
    auto it = game_items_.find(itemId);
    if (it != game_items_.end()) {
        return it->second;
    }
    throw std::runtime_error("Item not found: " + itemId);
}

void Pilgrimage::AddItemToPlayer(const std::string& itemId) {
    try {
        Item item = GenerateRandomItem();
        player_.AddItem(item);
        std::cout << "Item added: " << item.GetName() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void Pilgrimage::PrintGameOver() {
    system("cls");
    std::cout << "\n=== Game Over ===\n";

    if (player_.HasReachedTemple()) {
        std::cout << "You reached the temple in " << (player_.day() - 1) << " days!\n";
    }
    else if (!player_.IsAlive()) {
        std::cout << "You died...\n";
    }
    else if (!player_.HasWagon()) {
        std::cout << "Your wagon was destroyed!\n";
    }
    else {
        std::cout << "Time's up! Traveled: " << player_.distance_covered() << " km\n";
    }
}

int Pilgrimage::GetPlayerChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "> ";
        std::cin >> choice;

        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}