#pragma once
#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>
#include "Vector2.h"


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
private:
	Player* playerPtr_;
	std::array<PowerUp*,3> selectProducts_;
	uint8_t selectNum_ = 0;
	std::unordered_map<std::string,std::vector<std::unique_ptr<PowerUp>>> products_;
	std::vector<std::string> productKey_;
	std::string nowProductType;
	Vector2 boxLeftTop_ = {200,200};
	Vector2 boxSize_ = { 200,300 };
	int32_t boxDist_ = 400;
public:
	void Initialize();
	bool StatusChenge();
	void SetSlect(uint8_t selectNum);
	void SetPriducts();
	void SetPlayer(Player* player);
	void Draw();
};

