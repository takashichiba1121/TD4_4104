#include "DealDaemon.h"
#include "DxlibInclude.h"
#include <magic_enum.hpp>
#include <Player.h>
#include <json.hpp>
#include <fstream>
#include "CollisionManager.h"
#include <strconv.h>
#include "FontManager.h"

using namespace std;
using namespace nlohmann;

void DealDaemon::Initialize()
{

	selectmode_ = false;
	dealed_ = true;
	CollisionDisable();
	hitboxSize_ = { 128,128 };
	pos_ = { 650,640 };

	tag.tag = "Parts";

	userData_ = &tag;

	shape_ = new RectShape();
	shape_->SetRadius(hitboxSize_ / 2);
	CollisionManager::GetInstance()->AddObject(this);
	shape_->SetCenter(pos_);

	SetShape(shape_);

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

		temp->productType = static_cast< string >( obj[ "Type" ] );
		temp->partsName = static_cast<string>(obj["codeName"]);
		temp->cost = static_cast<int32_t>(obj["cost"]);

		temp->uiPartsName = utf8_to_sjis(static_cast< string >( obj[ "partsName" ] ));
		temp->partsInfo = utf8_to_sjis(static_cast<string>(obj["partsInfo"]));
		products_.push_back(std::move(temp));
	}
	nameFontHandle_ = FontManager::GetFontHandle("normal");
	infoFontHandle_ = FontManager::GetFontHandle("normal");
	

	SetPriducts();

}

void DealDaemon::Update()
{
	selectmode_ = playerPtr_->IsChangeParts();
	shape_->SetCenter(pos_);
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
			isBuy = playerPtr_->ChangeLeftArm(product->partsName,product->cost);
		}
		else
		{
			isBuy = playerPtr_->ChangeRightArm(product->partsName,product->cost);
		}
		break;
	case LEG:
		isBuy = playerPtr_->ChangeLeg(product->partsName,product->cost);
		break;
	case EYE:
		isBuy = playerPtr_->ChangeEye(product->partsName, product->cost);
		break;
	case MOUTH:
		isBuy = playerPtr_->ChangeMouth(product->partsName, product->cost);
		break;
	default:
		break;
	}
	dealCount_ = 0;
	dealed_ = isBuy;
	return isBuy;
}

bool DealDaemon::Deal()
{
	if (GetRand(1000) <= dealSucces_[dealCount_])
	{
		dealCount_++;
		dealCount_ = static_cast<int8_t>(min(dealCount_, dealSucces_.size() - 1));
		SetPriducts(true);
	}
	else
	{
		if ( GetRand(1000) <= statusDownIvent_ )
		{
			playerPtr_->IventDamage(playerPtr_->GetMaxHp() * 0.2f);
		}
		else
		{
			int32_t dice = GetRand(1000);
			if ( dice <= downStatus_[ 0 ] )
			{
				playerPtr_->SubPow(20);
			}
			else if ( dice <= downStatus_[ 1 ] )
			{
				playerPtr_->SubDef(15);
			}
			else if ( dice <= downStatus_[ 2 ] )
			{
				playerPtr_->SubSpd(10);
			}
			else
			{
				playerPtr_->SubMaxHp(30);
			}
		}
	}
	return false;
}

void DealDaemon::SetSlect(uint8_t selectNum)
{

	selectNum_ = selectNum;
}

void DealDaemon::SetPriducts(bool deal)
{
	for ( size_t i = 0; i < selectProducts_.size(); i++ )
	{
		selectProducts_[i] = nullptr;
	}

	for (size_t i = 0; i < selectProducts_.size(); i++)
	{
		Parts* temp = products_[ GetRand(products_.size() - 1) ].get();
		for ( size_t i = 0; i < selectProducts_.size(); i++ )
		{
			while ( playerPtr_->CheckHavePart(magic_enum::enum_cast< PartsName >( temp->productType ).value(),temp->partsName)
					|| temp == selectProducts_[i] )
			{
				temp = products_[ GetRand(products_.size() - 1) ].get();
			}
		}
		selectProducts_[ i ] = temp;
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

		for ( uint32_t i = 0; i < selectProducts_.size(); i++ )
		{
			int64_t color = 0x000000;
			if ( i == selectNum_ )
			{
				color = 0xf00f00;
			}
			DrawBox(( boxLeftTop_.x + i * boxDist_ ),boxLeftTop_.y,( boxLeftTop_.x + i * boxDist_ ) + boxSize_.x,boxLeftTop_.y + boxSize_.y,color,true);

			string nameTemp = selectProducts_[ i ]->uiPartsName;

			if ( selectProducts_[ i ]->productType == "ARM" )
			{
				if ( selectProducts_[ i ]->isLeft )
				{
					nameTemp += utf8_to_sjis("(左腕)");
				}
				else
				{
					nameTemp += utf8_to_sjis("(右腕)");
				}
			}

			DrawStringToHandle(( boxLeftTop_.x + i * boxDist_ ) + 50,boxLeftTop_.y + 50,nameTemp.c_str(),
			0xffffff,nameFontHandle_);

			string temp = utf8_to_sjis("価格:");
			temp += to_string(selectProducts_[ i ]->cost);

			DrawStringToHandle(( boxLeftTop_.x + i * boxDist_ ) + 50,boxLeftTop_.y + 75,temp.c_str(),
			0xffffff,infoFontHandle_);

			DrawStringToHandle(( boxLeftTop_.x + i * boxDist_ ) + 25,boxLeftTop_.y + 125,selectProducts_[ i ]->partsInfo.c_str(),
			0xffffff,infoFontHandle_);
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
