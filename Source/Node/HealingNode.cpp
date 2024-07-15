#include "HealingNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>

void HealingNode::Initialize()
{
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

}

void HealingNode::Update()
{
	Vector2 playerPos = mapChip_->GetPos(player_->GetPos().x,player_->GetPos().y);
	NextDoor nextDoor;

	if ( mapChip_->GetNumOfArrayElement(playerPos.x,playerPos.y + 1) == ChipIndex::NEXT )
	{
		for ( auto& door : nextdoors_ )
		{
			if ( door.pos.x == playerPos.x )
			{
				nextDoor = door;

				break;
			}
		}

		nodeManager_->ChangeNode(nextDoor.id);
	}

}

void HealingNode::Draw()
{
}

void HealingNode::Reset()
{
	player_->Reset();

	int32_t i = 0;
	int32_t count = 0;
	for ( auto& chip : mapChip_->GetMapChip()[ mapChip_->GetMapChip().size() - 2 ] )
	{
		if ( chip == ChipIndex::NEXT )
		{
			NextDoor nextDoor;
			nextDoor.pos = { float(i), float(mapChip_->GetMapChip().size() - 2) };
			nextDoor.id = count;
			nextdoors_.push_back(nextDoor);
			count++;
		}

		i++;
	}

	if ( nextdoors_.size() > nextDoorsNum_ )
	{
		std::vector<NextDoor> tmpNextdoors = nextdoors_;

		while ( tmpNextdoors.size() > nextDoorsNum_ && tmpNextdoors.size() != 1 )
		{
			NextDoor door = tmpNextdoors.back();
			mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::ROAD);
			tmpNextdoors.pop_back();
		}

	}

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
