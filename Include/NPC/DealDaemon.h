#pragma once

#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>
#include "Vector2.h"

class Player;

struct Parts
{
	std::string partsName;
	std::string uiPartsName;
	std::string partsInfo;
	int32_t cost;
};


enum PartsName
{
	ARM,
	LEG,
	EYE,
	MOUTH,
	BODY
};


class DealDaemon
{
private:
	Player* playerPtr_;
	std::array<Parts*,3> selectProducts_;
	uint8_t selectNum_ = 0;
	std::unordered_map<std::string,std::vector<std::unique_ptr<Parts>>> products_;
	std::vector<std::string> productKey_;
	std::string nowProductType;
	Vector2 boxLeftTop_ = { 200,200 };
	Vector2 boxSize_ = { 200,300 };
	int32_t boxDist_ = 400;
	int8_t dealCount;
	bool isLeft = false;
	const std::array<int32_t,7> dealSucces = { 950,750,500,200,100,50,20 };
public:
	void Initialize();
	bool PartsChenge();
	bool Deal();
	void SetSlect(uint8_t selectNum);
	void SetPriducts(bool deal = false);
	void SetPlayer(Player* player);
	void Draw();
};

