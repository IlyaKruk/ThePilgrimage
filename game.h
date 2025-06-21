#ifndef PILGRIMAGE_GAME_H_
#define PILGRIMAGE_GAME_H_

#include "player.h"
#include "enemy.h"
#include <vector>
#include <string>

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
	void PrintGameOver();
	int GetPlayerChoice(int min, int max);

	std::vector<std::string> enemy_types_;
	Enemy GenerateRandomEnemy() const;
	void CombatEncounter();

	Player player_;
	int actions_left_;
	bool is_day_;
};

#endif 