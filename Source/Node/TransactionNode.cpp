#include "TransactionNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void TransactionNode::Initialize()
{

}

void TransactionNode::Update()
{
	PlayerNodeMove();
	dealer_->Update();
}

void TransactionNode::Draw()
{
	dealer_->Draw();
}

void TransactionNode::Reset()
{
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	player_->Reset();
	dealer_->ReSet();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void TransactionNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;
	dealer_->NoDeal();
}

Vector2 TransactionNode::GetPlayerStartPos()
{
	return Vector2();
}
