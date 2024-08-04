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

struct Parts
{
	std::string partsName;
	std::string uiPartsName;
	std::string partsInfo;
	std::string productType;
	bool isLeft = false;
	int32_t cost;
};


enum PartsName
{
	ARM,
	LEG,
	EYE,
	MOUTH,
};


class DealDaemon : public BaseObject
{
private:
	int32_t nameFontHandle_ = 0;
	int32_t infoFontHandle_ = 0;
	Player* playerPtr_;
	std::array<Parts*,3> selectProducts_;
	uint8_t selectNum_ = 0;
	std::vector<std::unique_ptr<Parts>> products_;
	Vector2 boxLeftTop_ = { 100,200 };
	Vector2 boxSize_ = { 250,300 };
	int32_t boxDist_ = 400;
	int8_t dealCount_;
	bool isLeft_ = false;
	bool dealed_ = false;
	bool selectmode_ = false;
	RectShape* shape_;
	const std::array<int32_t,7> dealSucces_ = { 950,750,500,200,100,50,20 };
	int32_t statusDownIvent_ = 250;
	const std::array<int32_t,3> downStatus_ = {250,500,750};
	int32_t animeTimer_;
	int32_t anime_;
	int32_t animeNum_;
	int32_t animeSpeed_;
public:
	void Initialize() override;
	void Update() override;
	bool PartsChenge();
	bool Deal();
	void SetSlect(uint8_t selectNum);
	void SetPriducts(bool deal = false);
	void SetPlayer(Player* player);
	void Draw() override;
	void NoDeal();
	void ReSet();
	void AnimeUpdate(bool loop = true);
};

