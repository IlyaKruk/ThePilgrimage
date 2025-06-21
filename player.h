#ifndef PILGRIMAGE_PLAYER_H_
#define PILGRIMAGE_PLAYER_H_

#include <vector>

class Player {
public:
	Player();

	int health() const;
	int stamina() const;
	int gold() const;
	int day() const;
	int wagon_health() const;
	int days_left() const;
	int distance_covered() const;
	int distance_left() const;

	void TakeDamage(int damage);
	void Heal(int amount);
	void RestoreStamina(int amount);
	void SpendStamina(int amount);
	void AddGold(int amount);
	void SpendGold(int amount);
	void RepairWagon(int amount);
	void DamageWagon(int amount);
	void NextDay();
	void AddDistance(int km);

	bool IsAlive() const;
	bool HasWagon() const;
	bool HasTime() const;
	bool HasReachedTemple() const;

private:
	int health_;
	int stamina_;
	int gold_;
	int day_;
	int wagon_health_;
	int distance_covered_;

	const int MAX_HEALTH;
	const int MAX_STAMINA;
	const int MAX_DAYS;
	const int WAGON_HEALTH_MAX;
	const int DISTANCE_TO_TEMPLE;

	void InitConfig();
};

#endif 