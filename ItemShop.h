#pragma once
#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>
#include "Vector2.h"

class Player;

struct Item
{
	int32_t power;
	int32_t prise;
	std::string itemName;
	std::string statusName;
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

class ItemShop
{
private:
	Player* playerPtr_;
	std::vector<Item*> selectProducts_;
	uint8_t selectNum_ = 0;
	uint8_t shopNum_ = 0;
	std::vector<std::unique_ptr<Item>> products_;
	Vector2 boxLeftTop_ = { 200,200 };
	Vector2 boxSize_ = { 200,300 };
	int32_t boxDist_ = 400;
public:
	void Initialize();
	bool ItemSelect();
	void SetSlect(uint8_t selectNum);
	void SetPriducts();
	void SetPlayer(Player* player);
	void Draw();
};

