#ifndef PILGRIMAGE_GAME_H_
#define PILGRIMAGE_GAME_H_

#include "player.h"
#include "enemy.h"
#include "item.h"
#include <vector>
#include <string>
#include <unordered_map>

class Pilgrimage {
public:
	Pilgrimage();
	void Run();

private:
	void PrintWelcomeMessage() const;
	void Explore();
	void PrintStatus() const;
	void HandleDay();
	void HandleNight();
	void RepairWagon();
	void Rest();
	void DailyProgress();
	void DeepSleep();
	void DefendWagon();
	void NightMarch();
	void RandomTravelEvent();
	void HandleInventory();
	void AddItemToPlayer(const std::string& itemId);
	void PrintGameOver();
	int GetPlayerChoice(int min, int max);
	Item GetItem(const std::string& itemId) const;

	std::unordered_map<std::string, Item> game_items_;
	std::vector<std::string> enemy_types_;
	std::vector<std::string> item_types_;
	Enemy GenerateRandomEnemy() const;
    Item GenerateRandomItem() const;
	void CombatEncounter();

	Player player_;
	int actions_left_;
	bool is_day_;
};

#endif 