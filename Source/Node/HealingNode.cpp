#include "HealingNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void HealingNode::Initialize()
{
	
	healObj_.Initialize();
}

void HealingNode::Update()
{
	PlayerNodeMove();
	healObj_.Update();
}

void HealingNode::Draw()
{
	healObj_.Draw();
}

void HealingNode::Reset()
{

	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");
	player_->Reset();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void HealingNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;
}

Vector2 HealingNode::GetPlayerStartPos()
{
	return Vector2();
}
