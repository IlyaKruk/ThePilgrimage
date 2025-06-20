#include "game.h"
#include "player.h"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <stdlib.h>

Pilgrimage::Pilgrimage() :
    actions_left_(GameConfig::GetInstance().GetInt("day_duration", 3)),
    is_day_(true)
{
    std::srand(std::time(nullptr));
}

void Pilgrimage::PrintStatus() const {
    std::cout << "\n--- День " << player_.day() << " (" << player_.days_left()
        << " дней осталось) ---\n";
    std::cout << "Здоровье: " << player_.health() << "/" << GameConfig::GetInstance().GetInt("max_health", 100) << "\n";
    std::cout << "Выносливость: " << player_.stamina() << "/" << GameConfig::GetInstance().GetInt("max_stamina", 100) << "\n";
    std::cout << "Золото: " << player_.gold() << "\n";
    std::cout << "Состояние повозки: " << player_.wagon_health() << "/"
        << GameConfig::GetInstance().GetInt("wagon_health_max", 100) << "\n";
    std::cout << "Пройдено: " << player_.distance_covered() << " / "
        << GameConfig::GetInstance().GetInt("distance_to_temple", 1000) << " км\n";

    if (is_day_) {
        std::cout << "Дневных действий осталось: " << actions_left_ << "\n";
    }
    else {
        std::cout << "Ночь. Время отдыхать или защищать повозку.\n";
    }

    std::cout << "------------------------\n";
}

void Pilgrimage::HandleDay() {
    while (actions_left_ > 0 && player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        PrintStatus();

        std::cout << "Дневные действия (" << actions_left_ << " осталось):\n";
        std::cout << "1. Исследовать окрестности (-"
            << GameConfig::GetInstance().GetInt("explore_stamina_cost", 20)
            << " выносливости)\n";
        std::cout << "2. Починить повозку (-"
            << GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20)
            << " золота)\n";
        std::cout << "3. Отдохнуть (+"
            << GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40)
            << " выносливости)\n";
        std::cout << "4. Двигаться по тропе (+"
            << GameConfig::GetInstance().GetInt("daily_progress", 80)
            << " км, -" << GameConfig::GetInstance().GetInt("push_forward_cost", 30)
            << " выносливости)\n";

        int choice = GetPlayerChoice(1, 4);

        system("cls");

        switch (choice) {
        case 1: Explore(); break;
        case 2: RepairWagon(); break;
        case 3: Rest(); break;
        case 4: DailyProgress(); break;
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

        std::cout << "Ночные действия:\n";
        std::cout << "1. Крепко спать (+"
            << GameConfig::GetInstance().GetInt("rest_health_gain", 30)
            << " здоровья, риск атаки)\n";
        std::cout << "2. Дежурить (-"
            << GameConfig::GetInstance().GetInt("defend_stamina_cost", 30)
            << " выносливости, защита повозки)\n";
        std::cout << "3. Ночной марш (+"
            << GameConfig::GetInstance().GetInt("forced_march_bonus", 30)
            << " км, -" << GameConfig::GetInstance().GetInt("forced_march_cost", 40)
            << " выносливости, -" << GameConfig::GetInstance().GetInt("forced_march_health_cost", 10)
            << " здоровья)\n";

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
    std::cout << "Добро пожаловать в 'Pilgrimage'!\n";
    std::cout << "Вам нужно преодолеть " << player_.distance_left() << " км за "
        << GameConfig::GetInstance().GetInt("max_days", 14) << " дней.\n\n";
}

void Pilgrimage::Explore() {
    if (player_.stamina() < 20) {
        std::cout << "\nСлишком устали для исследования.\n";
        return;
    }

    player_.SpendStamina(20);
    actions_left_--;

    int event = rand() % 5;
    switch (event) {
    case 0:
        player_.AddGold(10 + rand() % 20);
        std::cout << "\nВы нашли пожертвования путников! Золото +" << player_.gold() << ".\n";
        break;
    case 1:
        player_.Heal(15);
        std::cout << "\nНашли целебные травы. Здоровье +15.\n";
        break;
    case 2:
        player_.DamageWagon(5);
        std::cout << "\nПопали в труднопроходимую местность. Повозка повреждена.\n";
        break;
    case 3:
        player_.RestoreStamina(15);
        std::cout << "\nНашли тихое красивое место. Выносливость +15.\n";
        break;
    case 4:
        std::cout << "\nДолгий путь не принес ничего особенного.\n";
        break;
    }
}

void Pilgrimage::RepairWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20);
    if (player_.gold() < cost) {
        std::cout << "Нужно " << cost << " золота!\n";
        return;
    }

    player_.SpendGold(cost);
    player_.RepairWagon(GameConfig::GetInstance().GetInt("wagon_repair_amount", 30));
    actions_left_--;
    std::cout << "Повозка отремонтирована!\n";
}

void Pilgrimage::Rest() {
    system("cls");
    int gain = GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40);
    player_.RestoreStamina(gain);
    actions_left_--;
    std::cout << "Отдохнули! +" << gain << " выносливости\n";
}

void Pilgrimage::DailyProgress() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("push_forward_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "Нужно " << cost << " выносливости!\n";
        return;
    }

    player_.SpendStamina(cost);
    player_.AddDistance(GameConfig::GetInstance().GetInt("daily_progress", 80));
    actions_left_--;
    std::cout << "Прошли +" << GameConfig::GetInstance().GetInt("daily_progress", 80) << " км!\n";

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}

void Pilgrimage::DeepSleep() {
    system("cls");
    player_.Heal(GameConfig::GetInstance().GetInt("rest_health_gain", 30));
    player_.RestoreStamina(GameConfig::GetInstance().GetInt("max_stamina", 100));
    std::cout << "Выспались! Полное восстановление\n";
    actions_left_--;

    if (rand() % 2 == 0) {
        int damage = GameConfig::GetInstance().GetInt("base_monster_damage", 15) + (rand() % 20);
        player_.DamageWagon(damage);
        std::cout << "Монстры атаковали! Повозка -" << damage << " прочности\n";
    }
}

void Pilgrimage::DefendWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("defend_stamina_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "Нужно " << cost << " выносливости!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;

    if (rand() % 10 < 7) {
        int damage = (GameConfig::GetInstance().GetInt("base_monster_damage", 15) / 2) + (rand() % 10);
        player_.DamageWagon(damage);
        std::cout << "Отбили атаку! Повозка -" << damage << " прочности\n";

        if (rand() % 3 == 0) {
            int damage = 5 + (rand() % 10);
            player_.TakeDamage(damage);
            std::cout << "Получили ранение: -" << damage << " здоровья\n";
        }
    }
}

void Pilgrimage::NightMarch() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("forced_march_cost", 40);
    if (player_.stamina() < cost) {
        std::cout << "Нужно " << cost << " выносливости!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;
    player_.TakeDamage(GameConfig::GetInstance().GetInt("forced_march_health_cost", 10));
    player_.AddDistance(GameConfig::GetInstance().GetInt("forced_march_bonus", 30));
    std::cout << "Ночной марш! +" << GameConfig::GetInstance().GetInt("forced_march_bonus", 30) << " км\n";

    if (rand() % 3 == 0) {
        int damage = 10 + (rand() % 15);
        player_.DamageWagon(damage);
        std::cout << "Повозка повреждена: -" << damage << " прочности\n";
    }

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}


void Pilgrimage::RandomTravelEvent() {
    system("cls");
    std::cout << "\n--- В пути ---\n";
    switch (rand() % 4) {
    case 0:
        player_.AddDistance(15);
        std::cout << "Нашли тропу! +15 км\n";
        break;
    case 1:
        player_.DamageWagon(10);
        std::cout << "Трудная дорога! Повозка -10\n";
        break;
    case 2:
        player_.Heal(15);
        std::cout << "Помогли путнику! +15 здоровья\n";
        break;
    case 3:
        player_.TakeDamage(10);
        player_.SpendGold(15);
        std::cout << "Ограблены! -10 здоровья, -15 золота\n";
        break;
    }
    std::cout << "---------------\n";
}

void Pilgrimage::PrintGameOver() {
    system("cls");
    std::cout << "\n=== Итог ===\n";

    if (player_.HasReachedTemple()) {
        std::cout << "Вы достигли храма за " << (player_.day() - 1) << " дней!\n";
    }
    else if (!player_.IsAlive()) {
        std::cout << "Вы погибли...\n";
    }
    else if (!player_.HasWagon()) {
        std::cout << "Повозка разрушена!\n";
    }
    else {
        std::cout << "Время вышло! Пройдено: " << player_.distance_covered() << " км\n";
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
            std::cout << "Неверный ввод. Попробуйте снова.\n";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}
