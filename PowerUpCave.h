#pragma once
#include <stdint.h>
#include <array>

class Player;

struct PowerUp
{
	int32_t power;
	int32_t cost;
	std::pair<int32_t,int32_t> powerRandRange;
	std::pair<int32_t,int32_t> costRandRange;
};

class PowerUpCave
{
	Player* playerPtr_;
	std::array<PowerUp,3> ploducts_;
	uint8_t selectNum_;
public:
	virtual void Initialize() = 0;
	virtual void PowerUp() = 0;
	void SetslectNum(uint8_t selectNum);
};

