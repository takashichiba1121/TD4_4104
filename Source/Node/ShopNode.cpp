#include "ShopNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void ShopNode::Initialize()
{

}

void ShopNode::Update()
{
	PlayerNodeMove();
}

void ShopNode::Draw()
{
}

void ShopNode::Reset()
{
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	player_->Reset();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void ShopNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;

}

Vector2 ShopNode::GetPlayerStartPos()
{
	return Vector2();
}
