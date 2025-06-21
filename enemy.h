#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy {
public:
    Enemy(const std::string& type);

    std::string GetName() const;
    int GetHealth() const;
    int GetDamage() const;
    int GetGoldReward() const;
    void TakeDamage(int amount);
    bool IsAlive() const;

private:
    std::string name_;
    int health_;
    int max_health_;
    int damage_;
    int gold_reward_;
};

#endif