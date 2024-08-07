#include "ItemShop.h"
#include "Player.h"
#include <magic_enum.hpp>
#include <json.hpp>
#include <fstream>
#include "DxlibInclude.h"

using namespace nlohmann;
using namespace std;

void ItemShop::Initialize()
{

	std::ifstream file;

	file.open("Resources/Deal/Power.json");

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;


	for ( json& obj : jsonObject )
	{
		unique_ptr<Item> temp = make_unique<Item>();
		temp->power = static_cast< int32_t >( obj[ "Power" ] );

		temp->prise = static_cast< int32_t >( obj[ "prise" ] );

		temp->time = static_cast< int32_t >( obj[ "Time" ] );

		temp->statusName = static_cast< string >( obj[ "statusName" ] );

		temp->itemName = static_cast< string >( obj[ "itemName" ] );

		products_.push_back(std::move(temp));
	}

	SetPriducts();
}

bool ItemShop::ItemSelect()
{
	Item* product = selectProducts_[ selectNum_ ];
	bool isBuy = false;
	isBuy = playerPtr_->ItemGet(*product);
	return isBuy;
}

void ItemShop::SetSlect(uint8_t selectNum)
{
	selectNum_ = min(selectNum,selectProducts_.size() - 1);
}

void ItemShop::SetPriducts()
{
	int inputItem = 0;
	selectProducts_.clear();
	for ( int i = 0; selectProducts_.size() < shopNum_; i++ )
	{
		if ( products_.size() < shopNum_ )
		{
			for ( int j = 0; j < products_.size(); j++ )
			{
				selectProducts_.push_back(products_[ j ].get());
			}
			break;
		}

		Item* temp = products_[ GetRand(products_.size() - 1) ].get();
		bool inputedItem = false;
		for ( int j = 0; j < selectProducts_.size(); j++ )
		{
			if ( temp == selectProducts_[ j ] )
			{
				inputedItem = true;
				break;
			}
		}

		if ( inputedItem )
		{
			continue;
		}

		selectProducts_.push_back(temp);
	}
}

void ItemShop::SetPlayer(Player* player)
{
	playerPtr_ = player;
}

void ItemShop::Draw()
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
			0xffffff,"%s\n値段:%d\n%s: +%d",selectProducts_[i]->itemName,selectProducts_[ i ]->prise,
			selectProducts_[ i ]->statusName,selectProducts_[ i ]->power);
	}
}
