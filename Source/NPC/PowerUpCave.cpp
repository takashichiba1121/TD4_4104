#include "PowerUpCave.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>
#include "DxlibInclude.h"
#include "CollisionManager.h"

void PowerUpCave::Initialize(std::string filePath)
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
	
	selectmode_ = playerPtr_->IsPowerUp();

}

void PowerUpCave::OnCollision()
{
}

	//Survival 生存
	//Hunt 攻撃
	//Precision クリティカル
}

void PowerUpCave::OnCollision()
{
}

bool PowerUpCave::StatusChenge()
{
	PowerUp* product = &selectProducts_[ selectNum_ ];
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
	selectNum_ = selectNum;
}
