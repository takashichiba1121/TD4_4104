#include "PowerUpCave.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>
#include "DxlibInclude.h"
#include "CollisionManager.h"

using namespace nlohmann;
using namespace std;

void PowerUpCave::Initialize()
{

	selectmode_ = false;
	dealed_ = false;
	hitboxSize_ = { 128,128 };
	pos_ = { 650,640 };

	shape_ = new RectShape();
	shape_->SetRadius(hitboxSize_ / 2);
	CollisionManager::GetInstance()->AddObject(this);
	shape_->SetCenter(pos_);

	SetShape(shape_);


	std::ifstream file;

	file.open("Resources/Deal/Power.json");

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;

	products_.clear();
	for ( json& obj : jsonObject)
	{
		unique_ptr<PowerUp> temp = make_unique<PowerUp>();
		temp->powerRandRange.first = static_cast< int32_t >( obj[ "PowerRange" ][ 0 ] );
		temp->powerRandRange.second = static_cast< int32_t >( obj[ "PowerRange" ][ 1 ] );

		temp->costRandRange.first = static_cast< int32_t >( obj[ "CostRange" ][ 0 ] );
		temp->costRandRange.second = static_cast< int32_t >( obj[ "CostRange" ][ 1 ] );

		temp->statusNames.first = static_cast< string >( obj[ "PowerName" ] );
		temp->statusNames.second = static_cast< string >( obj[ "CostName" ] );
		products_[ static_cast< string >( obj[ "Type" ] ) ].push_back(std::move(temp));
}

	for ( auto itr = products_.begin(); itr != products_.end(); ++itr )
	{
		string temp = itr->first;
		productKey_.push_back(temp);
		
	}

	SetPriducts();

	name_.tag = "PowerUpCave";
}


void PowerUpCave::Update()
{
	
	

}

void PowerUpCave::OnCollision()
{
}

bool PowerUpCave::StatusChenge()
{
	if ( dealed_ )
	{
		return true;
	}
	PowerUp* product = selectProducts_[ selectNum_ ];
	bool isBuy = false;
	switch ( magic_enum::enum_cast< Status >( product->statusNames.second ).value() )
	{
	case HP:
		isBuy = playerPtr_->SubMaxHp(product->cost);
		break;
	case ATK:
		isBuy = playerPtr_->SubPow(product->cost);
		break;
	case DEF:
		isBuy = playerPtr_->SubDef(product->cost);
		break;
	case SPD:
		isBuy = playerPtr_->SubSpd(product->cost);
		break;
	case CRIT:
		isBuy = playerPtr_->SubCrit(product->cost);
		break;
	case CDMG:
		isBuy = playerPtr_->SubCdmg(product->cost);
		break;
	default:
		break;
	}
	if ( isBuy )
	{
		switch ( magic_enum::enum_cast< Status >( product->statusNames.first ).value() )
		{
		case HP:
			isBuy = playerPtr_->AddMaxHp(product->power);
			break;
		case ATK:
			isBuy = playerPtr_->AddPow(product->power);
			break;
		case DEF:
			isBuy = playerPtr_->AddDef(product->power);
			break;
		case SPD:
			isBuy = playerPtr_->AddSpd(product->power);
			break;
		case CRIT:
			isBuy = playerPtr_->AddCrit(product->power);
			break;
		case CDMG:
			isBuy = playerPtr_->AddCdmg(product->power);
			break;
		default:
			break;
		}
	}
	dealed_ = isBuy;
	return isBuy;
}

void PowerUpCave::SetSlect(uint8_t selectNum)
{
	selectNum_ = min(selectNum,selectProducts_.size() - 1);
}

void PowerUpCave::SetPriducts()
{
	nowProductType = productKey_[ GetRand(productKey_.size() - 1) ];
	for ( int i = 0; i < selectProducts_.size(); i++ )
	{
		selectProducts_[ i ] = products_[ nowProductType ][ GetRand(products_[ nowProductType ].size()-1) ].get();

		int32_t change = selectProducts_[ i ]->powerRandRange.second - selectProducts_[ i ]->powerRandRange.first;
		selectProducts_[ i ]->power = GetRand(change) + selectProducts_[ i ]->powerRandRange.first;

		change = selectProducts_[ i ]->costRandRange.second - selectProducts_[ i ]->costRandRange.first;
		selectProducts_[ i ]->cost = GetRand(change) + selectProducts_[ i ]->costRandRange.first;
	}
}

void PowerUpCave::SetPlayer(Player* player)
{
	playerPtr_ = player;
}

void PowerUpCave::Draw()
{
	string type = productKey_[ GetRand(productKey_.size() - 1) ];
	for ( int i = 0; i < selectProducts_.size(); i++ )
	{
		int64_t color = 0x000000;
		if ( i == selectNum_ )
		{
			color = 0xf00f00;
		}
		DrawBox((boxLeftTop_.x + i * boxDist_),boxLeftTop_.y,( boxLeftTop_.x + i * boxDist_ ) + boxSize_.x,boxLeftTop_.y + boxSize_.y,color,true);
		DrawFormatString(( boxLeftTop_.x + i * boxDist_ ) + 50,boxLeftTop_.y + 50,
			0xffffff,"%s\nPowerUp\nStatus:%s \nUP:%d\nCost\nStatus:%s \nDown:%d",nowProductType.c_str(),selectProducts_[i]->statusNames.first.c_str(),
			selectProducts_[i]->power,selectProducts_[i]->statusNames.second.c_str(),selectProducts_[i]->cost);
	}
}

void PowerUpCave::SetPriducts()
{
	nowProductType = productKey_[ GetRand(productKey_.size() - 1) ];
	SRand(GetRand(INT_MAX));
	for ( int i = 0; i < selectProducts_.size(); i++ )
	{
		selectProducts_[ i ] = products_[ nowProductType ][ GetRand(products_[ nowProductType ].size()-1) ].get();

		int32_t change = selectProducts_[ i ]->powerRandRange.second - selectProducts_[ i ]->powerRandRange.first;
		selectProducts_[ i ]->power = GetRand(change) + selectProducts_[ i ]->powerRandRange.first;

		change = selectProducts_[ i ]->costRandRange.second - selectProducts_[ i ]->costRandRange.first;
		selectProducts_[ i ]->cost = GetRand(change) + selectProducts_[ i ]->costRandRange.first;
	}
}

void PowerUpCave::SetPlayer(Player* player)
{
	playerPtr_ = player;
}

void PowerUpCave::Draw()
{
	if ( selectmode_ && !dealed_)
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
				0xffffff,"%s\nPowerUp\nStatus:%s \nUP:%d\nCost\nStatus:%s \nDown:%d",nowProductType.c_str(),selectProducts_[ i ]->statusNames.first.c_str(),
				selectProducts_[ i ]->power,selectProducts_[ i ]->statusNames.second.c_str(),selectProducts_[ i ]->cost);
		}
	}

	if ( !dealed_ )
	{
		DrawBox(pos_.x - hitboxSize_.x/2,pos_.y - hitboxSize_.y/2,pos_.x + hitboxSize_.x/2,pos_.y + hitboxSize_.y/2,0xffffff,true);
	}
}
