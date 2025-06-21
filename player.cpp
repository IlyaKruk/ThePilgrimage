#include "player.h"
#include "config.h"
#include <algorithm>
#include <iostream>

Player::Player() :
    MAX_HEALTH(GameConfig::GetInstance().GetInt("max_health", 100)),
    MAX_STAMINA(GameConfig::GetInstance().GetInt("max_stamina", 100)),
    MAX_DAYS(GameConfig::GetInstance().GetInt("max_days", 14)),
    WAGON_HEALTH_MAX(GameConfig::GetInstance().GetInt("wagon_health_max", 100)),
    DISTANCE_TO_TEMPLE(GameConfig::GetInstance().GetInt("distance_to_temple", 1000)),
    damage_boost_(GameConfig::GetInstance().GetInt("damage_boost", 0)),
    health_(GameConfig::GetInstance().GetInt("max_health", 100)),
    stamina_(GameConfig::GetInstance().GetInt("max_stamina", 100)),
    gold_(0),
    day_(1),
    wagon_health_(GameConfig::GetInstance().GetInt("wagon_health_max", 100)),
    distance_covered_(0) {
}

int Player::health() const { return health_; }
int Player::stamina() const { return stamina_; }
int Player::damage_boost() const { return damage_boost_; }
int Player::gold() const { return gold_; }
int Player::day() const { return day_; }
int Player::wagon_health() const { return wagon_health_; }
int Player::days_left() const { return MAX_DAYS - day_ + 1; }
int Player::distance_covered() const { return distance_covered_; }
int Player::distance_left() const { return DISTANCE_TO_TEMPLE - distance_covered_; }


void Player::TakeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

void Player::Heal(int amount) {
    health_ += amount;
    if (health_ > MAX_HEALTH) health_ = MAX_HEALTH;
}

void Player::RestoreStamina(int amount) {
    stamina_ += amount;
    if (stamina_ > MAX_STAMINA) stamina_ = MAX_STAMINA;
}

void Player::SpendStamina(int amount) {
    stamina_ -= amount;
    if (stamina_ < 0) stamina_ = 0;
}

void Player::AddGold(int amount) { gold_ += amount; }

void Player::SpendGold(int amount) { 
    gold_ -= amount; 
    if (gold_ < 0) gold_ = 0;
}

void Player::RepairWagon(int amount) {
    wagon_health_ += amount;
    if (wagon_health_ > WAGON_HEALTH_MAX) wagon_health_ = WAGON_HEALTH_MAX;
}

void Player::DamageWagon(int amount) {
    wagon_health_ -= amount;
    if (wagon_health_ < 0) wagon_health_ = 0;
}

void Player::NextDay() { ++day_; }

void Player::AddDistance(int km) { distance_covered_ += km; }

bool Player::IsAlive() const { return health_ > 0; }

bool Player::HasWagon() const { return wagon_health_ > 0; }

bool Player::HasTime() const { return day_ <= MAX_DAYS; }

bool Player::HasReachedTemple() const { return distance_covered_ >= DISTANCE_TO_TEMPLE; }

void Player::AddItem(const Item& item) {
    inventory_.push_back(item);
}

const std::vector<Item>& Player::GetInventory() const {
    return inventory_;
}

void Player::InitConfig()
{
}
