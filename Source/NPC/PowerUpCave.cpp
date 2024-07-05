#include "PowerUpCave.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>
#include "DxLib.h"

using namespace nlohmann;
using namespace std;

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


	for ( json& obj : jsonObject[ "objects" ] )
	{
		unique_ptr<PowerUp> temp = make_unique<PowerUp>();
		temp->power = static_cast< int32_t >(obj[ "Power" ]);
		temp->cost = static_cast< int32_t >(obj[ "Cost" ]);
		temp->powerRandRange.first = static_cast< int32_t >( obj[ "PowerRange" ][ 0 ] );
		temp->powerRandRange.second = static_cast< int32_t >( obj[ "PowerRange" ][ 1 ] );

		temp->costRandRange.first = static_cast< int32_t >( obj[ "CostRange" ][ 0 ] );
		temp->costRandRange.second = static_cast< int32_t >( obj[ "CostRange" ][ 1 ] );

		temp->statusNames.first = static_cast< string >( obj[ "PowerName" ] );
		temp->statusNames.second = static_cast< string >( obj[ "CostName" ] );
		products[ static_cast< string >( obj[ "Type" ] ) ].push_back(std::move(temp));
	}

	for ( auto itr = products.begin(); itr != products.end(); ++itr )
	{
		string temp = itr->first;
		productKey.push_back(temp);
		
	}
}

bool PowerUpCave::StatusChenge()
{
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
	return isBuy;
}

void PowerUpCave::SetSlect(uint8_t selectNum)
{
	selectNum_ = selectNum;
}

void PowerUpCave::SetPriducts()
{
	string type = productKey[ GetRand(productKey.size() - 1) ];
	for ( int i = 0; i < selectProducts_.size(); i++ )
	{
		selectProducts_[ i ] = products[ type ][ GetRand(products[ type ].size()-1) ].get();
	}
}
