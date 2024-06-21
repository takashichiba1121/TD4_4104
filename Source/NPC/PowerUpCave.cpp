#include "PowerUpCave.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>

void PowerUpCave::Initialize(std::string filePath)
{

	std::ifstream file;

	file.open("Resources/Deal/Power.json");

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;



	//Survival 生存
	//Hunt 攻撃
	//Precision クリティカル
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
			break;
		default:
			break;
		}
	}
	return isBuy;
}

void PowerUpCave::SetSlect(uint8_t selectNum)
{
	selectNum_ = selectNum;
}
