#include "PowerUpCave.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>
#include "DxlibInclude.h"
#include "CollisionManager.h"
#include <strconv.h>
#include "FontManager.h"

using namespace nlohmann;
using namespace std;

void PowerUpCave::Initialize()
{

	selectmode_ = false;
	dealed_ = true;
	CollisionDisable();
	hitboxSize_ = { 128,128 };
	pos_ = { 650,600 };

	tex_ = LoadGraph(string("Resources\\Node\\reinforcement.png"));

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
	font_ = FontManager::GetFontHandle("normal");
	tag.tag = "PowerUp";
	userData_ = &tag;
}


void PowerUpCave::Update()
{
	
	selectmode_ = playerPtr_->IsPowerUp();
	shape_->SetCenter(pos_);
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
	selectNum_ = static_cast< uint8_t >(min(static_cast<size_t>(selectNum),selectProducts_.size() - 1));
}

void PowerUpCave::SetPriducts()
{
	nowProductType = productKey_[ GetRand(productKey_.size() - 1) ];
	for ( size_t i = 0; i < selectProducts_.size(); i++ )
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
	for ( size_t i = 0; i < selectProducts_.size(); i++ )
	{
		int64_t color = 0x000000;
		if ( i == selectNum_ )
		{
			color = 0xf00f00;
		}
			DrawBox(( boxLeftTop_.x + i * boxDist_ ),boxLeftTop_.y,( boxLeftTop_.x + i * boxDist_ ) + boxSize_.x,boxLeftTop_.y + boxSize_.y,color,true);
			string temp;
			if ( nowProductType == "Attack" )
			{
				temp = "狩猟の道";
			}
			else if ( nowProductType == "survival" )
			{
				temp = "生存の道";
			}
			else
			{
				temp = "精密の道";
			}
			temp = temp + "\n\n強化\nステータス:" + selectProducts_[ i ]->statusNames.first +
				"\n増加量:"+ to_string(selectProducts_[ i ]->power) + "\n\n\n代償\nステータス: "+
				selectProducts_[ i ]->statusNames.second + "\n減少量:"+ to_string(selectProducts_[ i ]->cost);

			temp = utf8_to_sjis(temp);
			DrawStringToHandle(( boxLeftTop_.x + i * boxDist_ ) + 50,boxLeftTop_.y + 50,temp.c_str(),
				0xffffff,font_);
		}
	}

	if ( !dealed_ )
	{
		if ( nowProductType == "Attack")
		{
			SetDrawBright(0xf1,0x00,0x00);
		}
		else if ( nowProductType == "survival" )
		{
			SetDrawBright(0x00,0xf1,0x00);
		}
		else
		{
			SetDrawBright(0xf1,0xf1,0x00);
		}


		DrawRotaGraph(pos_.x,pos_.y,2,0,tex_,true);
		SetDrawBright(0xff,0xff,0xff);
	}
}

void PowerUpCave::ReSet()
{
	selectmode_ = false;
	dealed_ = false;
	hitboxSize_ = { 128,128 };
	pos_ = { 650,640 };
	SetPriducts();
	CollisionEnable();
}

void PowerUpCave::NoDeal()
{
	dealed_ = true;
	selectmode_ = false;
	CollisionDisable();
}
