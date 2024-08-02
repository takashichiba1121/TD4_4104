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
		temp->uiPartsName = static_cast< string >( obj[ "partsName" ] );
		temp->partsName = static_cast<string>(obj["codeName"]);
		temp->partsInfo = static_cast<string>(obj["partsInfo"]);
		temp->productType = static_cast< string >( obj[ "Type" ] );
		products_.push_back(std::move(temp));
	}

	SetPriducts();

}

bool DealDaemon::PartsChenge()
{

	Parts* product = selectProducts_[selectNum_];
	bool isBuy = false;
	switch (magic_enum::enum_cast<PartsName>( product ->productType).value())
	{
	case ARM:
		if ( product ->isLeft )
		{
			playerPtr_->ChangeLeftArm(product->partsName,product->cost);
		}
		else
		{
			playerPtr_->ChangeRightArm(product->partsName,product->cost);
		}
		break;
	case LEG:
		playerPtr_->ChangeLeg(product->partsName,product->cost);
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

	for (int i = 0; i < selectProducts_.size(); i++)
	{
		selectProducts_[i] = products_[GetRand(products_.size() - 1)].get();
		if ( selectProducts_[ i ]->productType == "ARM" )
		{
			if ( GetRand(10) <= 5 )
			{
				selectProducts_[ i ]->isLeft = true;
			}
			else
			{
				selectProducts_[ i ]->isLeft = false;
			}
		}
	}

}

void DealDaemon::SetPlayer(Player* player)
{
	playerPtr_ = player;
}

void DealDaemon::Draw()
{
	if ( selectmode_ && !dealed_ )
	{

		for ( int i = 0; i < selectProducts_.size(); i++ )
		{
			int64_t color = 0x000000;
			if ( i == selectNum_ )
			{
				color = 0xf00f00;
			}
			DrawBox(( boxLeftTop_.x + i * boxDist_ ),boxLeftTop_.y,( boxLeftTop_.x + i * boxDist_ ) + boxSize_.x,boxLeftTop_.y + boxSize_.y,color,true);
			DrawFormatString(( boxLeftTop_.x + i * boxDist_ ) + 50,boxLeftTop_.y + 50,
				0xffffff,"%s\nCost:%d\n%s",selectProducts_[ i ]->uiPartsName,selectProducts_[ i ]->cost,selectProducts_[ i ]->partsInfo);
		}
	}

	if ( !dealed_ )
	{
		DrawBox(pos_.x - hitboxSize_.x / 2,pos_.y - hitboxSize_.y / 2,pos_.x + hitboxSize_.x / 2,pos_.y + hitboxSize_.y / 2,0xffffff,true);
	}

}
void DealDaemon::NoDeal()
{
	dealed_ = true;
	selectmode_ = false;
	CollisionDisable();
}
void DealDaemon::ReSet()
{
	selectmode_ = false;
	dealed_ = false;
	hitboxSize_ = { 128,128 };
	pos_ = { 650,640 };
	SetPriducts();
	CollisionEnable();
}
