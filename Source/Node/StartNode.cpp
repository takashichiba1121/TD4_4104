#include "StartNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void StartNode::Initialize()
{
}

void StartNode::Update()
{
	PlayerNodeMove();
}

void StartNode::Draw()
{
}

void StartNode::Reset()
{
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	player_->Reset();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void StartNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;
}

Vector2 StartNode::GetPlayerStartPos()
{
    return Vector2();
}
