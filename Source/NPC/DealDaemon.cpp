#include "DealDaemon.h"
#include "DxlibInclude.h"
#include <magic_enum.hpp>
#include <Player.h>
#include <json.hpp>
#include <fstream>

using namespace std;
using namespace nlohmann;

void DealDaemon::Initialize()
{
	std::ifstream file;

	file.open("Resources/Deal/Parts.json");

	if (file.fail())
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;


	for (json& obj : jsonObject)
	{
		unique_ptr<Parts> temp = make_unique<Parts>();

		temp->cost = static_cast<int32_t>(obj["cost"]);
		temp->partsName = static_cast<string>(obj["partsName"]);

		products_[static_cast<string>(obj["Type"])].push_back(std::move(temp));
	}

	for (auto itr = products_.begin(); itr != products_.end(); ++itr)
	{
		string temp = itr->first;
		productKey_.push_back(temp);

	}

	SetPriducts();

}

bool DealDaemon::PartsChenge()
{

	Parts* product = selectProducts_[selectNum_];
	bool isBuy = false;
	switch (magic_enum::enum_cast<PartsName>(nowProductType).value())
	{
	case LARM:
		playerPtr_->ChangeLeftArm(product->partsName);
		break;
	case RARM:
		playerPtr_->ChangeRightArm(product->partsName);
		break;
	case LEG:
		playerPtr_->ChangeLeg(product->partsName);
		break;
	case EYE:

		break;
	case MOUTH:

		break;
	case BODY:

		break;
	default:
		break;
	}
	dealCount = 0;
	return true;
}

bool DealDaemon::Deal()
{
	if (GetRand(1000) <= dealSucces[dealCount])
	{
		dealCount++;
		dealCount = min(dealCount, dealSucces.size() - 1);
		SetPriducts(true);
	}
	return false;
}

void DealDaemon::SetSlect(uint8_t selectNum)
{

	selectNum_ = selectNum;
}

void DealDaemon::SetPriducts(bool deal)
{
	if (deal)
	{
		string type = productKey_[GetRand(productKey_.size() - 1)];
		while (nowProductType == type)
		{
			type = productKey_[GetRand(productKey_.size() - 1)];
		}
	}
	else
	{
		nowProductType = productKey_[GetRand(productKey_.size() - 1)];
	}

	for (int i = 0; i < selectProducts_.size(); i++)
	{
		selectProducts_[i] = products_[nowProductType][GetRand(products_[nowProductType].size() - 1)].get();
	}

}

void DealDaemon::SetPlayer(Player* player)
{
	playerPtr_ = player;
}

void DealDaemon::Draw()
{
	string type = productKey_[GetRand(productKey_.size() - 1)];
	for (int i = 0; i < selectProducts_.size(); i++)
	{
		int64_t color = 0x000000;
		if (i == selectNum_)
		{
			color = 0xf00f00;
		}
		DrawBox((boxLeftTop_.x + i * boxDist_), boxLeftTop_.y, (boxLeftTop_.x + i * boxDist_) + boxSize_.x, boxLeftTop_.y + boxSize_.y, color, true);
		DrawFormatString((boxLeftTop_.x + i * boxDist_) + 50, boxLeftTop_.y + 50,
			0xffffff, " ");
	}

}
