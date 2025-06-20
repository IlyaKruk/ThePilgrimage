#ifndef PILGRIMAGE_GAME_H_
#define PILGRIMAGE_GAME_H_

#include "player.h"

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

	Player player_;
	int actions_left_;
	bool is_day_;
};

#endif 