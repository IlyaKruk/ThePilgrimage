#include "enemy.h"
#include "config.h"

Enemy::Enemy() :
	enemy_health_(GameConfig::GetInstance().GetInt("max_health", 100)){
}

int Enemy::enemy_health() const { return enemy_health_; }
