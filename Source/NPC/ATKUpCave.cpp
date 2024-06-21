#include "ATKUpCave.h"
#include <magic_enum.hpp>

#include "Player.h"

void ATKUpCave::Initialize()
{



}

bool ATKUpCave::StatusChenge()
{
	PowerUp* product = &products_[selectNum_];
	switch (magic_enum::enum_cast< Status >( product->statusNames.second ).value())
	{
	case HP:
		playerPtr_->SubMaxHp(product->cost);
		break;
	case ATK:
		playerPtr_->SubPow(product->cost);
		break;
	case DEF:
		playerPtr_->SubDef(product->cost);
		break;
	case SPD:
		playerPtr_->SubSpd(product->cost);
		break;
	case CRIT:

		break;
	case CDMG:
		break;
	default:
		break;
	}
}
