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
    std::cout << "\n--- ���� " << player_.day() << " (" << player_.days_left()
        << " ���� ��������) ---\n";
    std::cout << "��������: " << player_.health() << "/" << GameConfig::GetInstance().GetInt("max_health", 100) << "\n";
    std::cout << "������������: " << player_.stamina() << "/" << GameConfig::GetInstance().GetInt("max_stamina", 100) << "\n";
    std::cout << "������: " << player_.gold() << "\n";
    std::cout << "��������� �������: " << player_.wagon_health() << "/"
        << GameConfig::GetInstance().GetInt("wagon_health_max", 100) << "\n";
    std::cout << "��������: " << player_.distance_covered() << " / "
        << GameConfig::GetInstance().GetInt("distance_to_temple", 1000) << " ��\n";

    if (is_day_) {
        std::cout << "������� �������� ��������: " << actions_left_ << "\n";
    }
    else {
        std::cout << "����. ����� �������� ��� �������� �������.\n";
    }

    std::cout << "------------------------\n";
}

void Pilgrimage::HandleDay() {
    while (actions_left_ > 0 && player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        PrintStatus();

        std::cout << "������� �������� (" << actions_left_ << " ��������):\n";
        std::cout << "1. ����������� ����������� (-"
            << GameConfig::GetInstance().GetInt("explore_stamina_cost", 20)
            << " ������������)\n";
        std::cout << "2. �������� ������� (-"
            << GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20)
            << " ������)\n";
        std::cout << "3. ��������� (+"
            << GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40)
            << " ������������)\n";
        std::cout << "4. ��������� �� ����� (+"
            << GameConfig::GetInstance().GetInt("daily_progress", 80)
            << " ��, -" << GameConfig::GetInstance().GetInt("push_forward_cost", 30)
            << " ������������)\n";

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

        std::cout << "������ ��������:\n";
        std::cout << "1. ������ ����� (+"
            << GameConfig::GetInstance().GetInt("rest_health_gain", 30)
            << " ��������, ���� �����)\n";
        std::cout << "2. �������� (-"
            << GameConfig::GetInstance().GetInt("defend_stamina_cost", 30)
            << " ������������, ������ �������)\n";
        std::cout << "3. ������ ���� (+"
            << GameConfig::GetInstance().GetInt("forced_march_bonus", 30)
            << " ��, -" << GameConfig::GetInstance().GetInt("forced_march_cost", 40)
            << " ������������, -" << GameConfig::GetInstance().GetInt("forced_march_health_cost", 10)
            << " ��������)\n";

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
    std::cout << "����� ���������� � 'Pilgrimage'!\n";
    std::cout << "��� ����� ���������� " << player_.distance_left() << " �� �� "
        << GameConfig::GetInstance().GetInt("max_days", 14) << " ����.\n\n";
}

void Pilgrimage::Explore() {
    if (player_.stamina() < 20) {
        std::cout << "\n������� ������ ��� ������������.\n";
        return;
    }

    player_.SpendStamina(20);
    actions_left_--;

    int event = rand() % 5;
    switch (event) {
    case 0:
        player_.AddGold(10 + rand() % 20);
        std::cout << "\n�� ����� ������������� ��������! ������ +" << player_.gold() << ".\n";
        break;
    case 1:
        player_.Heal(15);
        std::cout << "\n����� �������� �����. �������� +15.\n";
        break;
    case 2:
        player_.DamageWagon(5);
        std::cout << "\n������ � ���������������� ���������. ������� ����������.\n";
        break;
    case 3:
        player_.RestoreStamina(15);
        std::cout << "\n����� ����� �������� �����. ������������ +15.\n";
        break;
    case 4:
        std::cout << "\n������ ���� �� ������ ������ ����������.\n";
        break;
    }
}

void Pilgrimage::RepairWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("base_wagon_repair_cost", 20);
    if (player_.gold() < cost) {
        std::cout << "����� " << cost << " ������!\n";
        return;
    }

    player_.SpendGold(cost);
    player_.RepairWagon(GameConfig::GetInstance().GetInt("wagon_repair_amount", 30));
    actions_left_--;
    std::cout << "������� ���������������!\n";
}

void Pilgrimage::Rest() {
    system("cls");
    int gain = GameConfig::GetInstance().GetInt("day_rest_stamina_gain", 40);
    player_.RestoreStamina(gain);
    actions_left_--;
    std::cout << "���������! +" << gain << " ������������\n";
}

void Pilgrimage::DailyProgress() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("push_forward_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "����� " << cost << " ������������!\n";
        return;
    }

    player_.SpendStamina(cost);
    player_.AddDistance(GameConfig::GetInstance().GetInt("daily_progress", 80));
    actions_left_--;
    std::cout << "������ +" << GameConfig::GetInstance().GetInt("daily_progress", 80) << " ��!\n";

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}

void Pilgrimage::DeepSleep() {
    system("cls");
    player_.Heal(GameConfig::GetInstance().GetInt("rest_health_gain", 30));
    player_.RestoreStamina(GameConfig::GetInstance().GetInt("max_stamina", 100));
    std::cout << "���������! ������ ��������������\n";
    actions_left_--;

    if (rand() % 2 == 0) {
        int damage = GameConfig::GetInstance().GetInt("base_monster_damage", 15) + (rand() % 20);
        player_.DamageWagon(damage);
        std::cout << "������� ���������! ������� -" << damage << " ���������\n";
    }
}

void Pilgrimage::DefendWagon() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("defend_stamina_cost", 30);
    if (player_.stamina() < cost) {
        std::cout << "����� " << cost << " ������������!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;

    if (rand() % 10 < 7) {
        int damage = (GameConfig::GetInstance().GetInt("base_monster_damage", 15) / 2) + (rand() % 10);
        player_.DamageWagon(damage);
        std::cout << "������ �����! ������� -" << damage << " ���������\n";

        if (rand() % 3 == 0) {
            int damage = 5 + (rand() % 10);
            player_.TakeDamage(damage);
            std::cout << "�������� �������: -" << damage << " ��������\n";
        }
    }
}

void Pilgrimage::NightMarch() {
    system("cls");
    int cost = GameConfig::GetInstance().GetInt("forced_march_cost", 40);
    if (player_.stamina() < cost) {
        std::cout << "����� " << cost << " ������������!\n";
        return;
    }

    player_.SpendStamina(cost);
    actions_left_--;
    player_.TakeDamage(GameConfig::GetInstance().GetInt("forced_march_health_cost", 10));
    player_.AddDistance(GameConfig::GetInstance().GetInt("forced_march_bonus", 30));
    std::cout << "������ ����! +" << GameConfig::GetInstance().GetInt("forced_march_bonus", 30) << " ��\n";

    if (rand() % 3 == 0) {
        int damage = 10 + (rand() % 15);
        player_.DamageWagon(damage);
        std::cout << "������� ����������: -" << damage << " ���������\n";
    }

    if (player_.IsAlive() && player_.HasWagon() && !player_.HasReachedTemple()) {
        RandomTravelEvent();
    }
}


void Pilgrimage::RandomTravelEvent() {
    system("cls");
    std::cout << "\n--- � ���� ---\n";
    switch (rand() % 4) {
    case 0:
        player_.AddDistance(15);
        std::cout << "����� �����! +15 ��\n";
        break;
    case 1:
        player_.DamageWagon(10);
        std::cout << "������� ������! ������� -10\n";
        break;
    case 2:
        player_.Heal(15);
        std::cout << "������� �������! +15 ��������\n";
        break;
    case 3:
        player_.TakeDamage(10);
        player_.SpendGold(15);
        std::cout << "���������! -10 ��������, -15 ������\n";
        break;
    }
    std::cout << "---------------\n";
}

void Pilgrimage::PrintGameOver() {
    system("cls");
    std::cout << "\n=== ���� ===\n";

    if (player_.HasReachedTemple()) {
        std::cout << "�� �������� ����� �� " << (player_.day() - 1) << " ����!\n";
    }
    else if (!player_.IsAlive()) {
        std::cout << "�� �������...\n";
    }
    else if (!player_.HasWagon()) {
        std::cout << "������� ���������!\n";
    }
    else {
        std::cout << "����� �����! ��������: " << player_.distance_covered() << " ��\n";
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
            std::cout << "�������� ����. ���������� �����.\n";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}
