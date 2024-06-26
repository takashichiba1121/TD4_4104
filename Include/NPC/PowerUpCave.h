#pragma once
#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>


class Player;

struct PowerUp
{
	int32_t power;
	int32_t cost;
	std::pair<int32_t,int32_t> powerRandRange;
	std::pair<int32_t,int32_t> costRandRange;
	std::pair<std::string,std::string> statusNames;
};

enum Status
{
	HP,
	ATK,
	DEF,
	SPD,
	CRIT,
	CDMG
};


class PowerUpCave
{
protected:
	Player* playerPtr_;
	std::array<PowerUp*,3> selectProducts_;
	uint8_t selectNum_;
	std::unordered_map<std::string,std::vector<PowerUp>> products;
public:
	void Initialize(std::string filePath);
	bool StatusChenge();
	void SetSlect(uint8_t selectNum);
	void SetPriducts();
	void SetPlayer(Player* player);
};

