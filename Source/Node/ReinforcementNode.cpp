#include "ReinforcementNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void ReinforcementNode::Initialize()
{
	
}

void ReinforcementNode::Update()
{
	PlayerNodeMove();
	powerUp_->Update();
}

void ReinforcementNode::Draw()
{
	powerUp_->Draw();
}

void ReinforcementNode::Reset()
{
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	player_->Reset();
	powerUp_->ReSet();
	
	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void ReinforcementNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;
	powerUp_->NoDeal();
}

Vector2 ReinforcementNode::GetPlayerStartPos()
{
	return Vector2();
}
