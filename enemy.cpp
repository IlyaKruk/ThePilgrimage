#include "enemy.h"
#include "config.h"
#include <iostream>

Enemy::Enemy(const std::string& type) {
    name_ = GameConfig::GetInstance().GetString("enemy_" + type + "_name", type);
    max_health_ = GameConfig::GetInstance().GetInt("enemy_" + type + "_health", 30);
    health_ = max_health_;
    damage_ = GameConfig::GetInstance().GetInt("enemy_" + type + "_damage", 10);
    gold_reward_ = GameConfig::GetInstance().GetInt("enemy_" + type + "_gold", 15);
}

std::string Enemy::GetName() const {
    return name_;
}

int Enemy::GetHealth() const {
    return health_;
}

int Enemy::GetDamage() const {
    return damage_;
}

int Enemy::GetGoldReward() const {
    return gold_reward_;
}

void Enemy::TakeDamage(int amount) {
    health_ -= amount;
    if (health_ < 0) {
        health_ = 0;
    }

    std::cout << name_ << " получает " << amount << " урона! ";
    if (health_ > 0) {
        std::cout << "Осталось " << health_ << "/" << max_health_ << " здоровья.\n";
    }
    else {
        std::cout << name_ << " повержен!\n";
    }
}

bool Enemy::IsAlive() const {
    return health_ > 0;
}