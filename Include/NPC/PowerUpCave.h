#pragma once
#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>
#include "Vector2.h"
#include "BaseObject.h"


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


class PowerUpCave:public BaseObject
{
private:
	Player* playerPtr_;
	int32_t font_;
	std::array<PowerUp*,3> selectProducts_;
	std::array<int32_t,3> color_ = {0xf10000,0x00f100,0xf1f100};
	uint8_t selectNum_ = 0;
	std::unordered_map<std::string,std::vector<std::unique_ptr<PowerUp>>> products_;
	std::vector<std::string> productKey_;
	std::string nowProductType;	Vector2 boxLeftTop_ = {200,200};
	Vector2 boxSize_ = { 200,300 };
	int32_t boxDist_ = 400;
	int32_t caveColor;
	bool dealed_ = false;
	bool selectmode_ = false;
	RectShape* shape_;
public:
	void Initialize() override;
	void Update() override;
	void OnCollision() override;
	void Draw() override;
	void ReSet();
	void NoDeal();
	bool StatusChenge();
	void SetSlect(uint8_t selectNum);
	void SetPriducts();
	void SetPlayer(Player* player);
};

