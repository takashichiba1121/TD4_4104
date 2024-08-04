#include "BaseNode.h"
#include<cassert>
#include<NodeManager.h>

void BaseNode::SetNode(Node* node)
{
	assert(node);

	position_ = node->position;
	row_ = node->row;
	column_ = node->column;
	type_ = node->type;
	nextDoorsNum_ = node->nextDoorsNum;
}

void BaseNode::SetMapChip(MapChip* mapChip)
{
	mapChip_ = mapChip;
}

void BaseNode::SetPlayer(Player* player)
{
	player_ = player;
}

void BaseNode::SetPowerUp(PowerUpCave* powerUp)
{
	powerUp_ = powerUp;
}

void BaseNode::SetDealer(DealDaemon* dealer)
{
	dealer_ = dealer;
}

void BaseNode::SetNodeManagerr(NodeManager* nodeManager)
{
	nodeManager_ = nodeManager;
}

void BaseNode::SetEnemyManager(EnemyManager* enemyManager)
{
	enemys_ = enemyManager;
}

void BaseNode::SetScrollStop(bool* scrollStop)
{
	isScrollStop_ = scrollStop;
}

void BaseNode::PlayerNodeMove()
{
	Vector2 playerPos = mapChip_->GetPos(player_->GetPos().x+37,player_->GetPos().y+16);
	NextDoor* nextDoor = nullptr;

	if ( mapChip_->GetNumOfArrayElement(playerPos.x,playerPos.y + 1) == ChipIndex::NEXT ||
		mapChip_->GetNumOfArrayElement(playerPos.x,playerPos.y) == ChipIndex::NEXT )
	{
		for ( auto& door : nextdoors_ )
		{
			if ( door.pos.x == playerPos.x )
			{
				nextDoor = &door;

				break;
			}
		}
	}
	else if ( mapChip_->GetNumOfArrayElement(playerPos.x - 1,playerPos.y) == ChipIndex::NEXT )
	{
		for ( auto& door : nextdoors_ )
		{
			if ( door.pos.x == playerPos.x - 1 )
			{
				nextDoor = &door;

				break;
			}
		}
	}
	else if ( mapChip_->GetNumOfArrayElement(playerPos.x + 1,playerPos.y) == ChipIndex::NEXT )
	{
		for ( auto& door : nextdoors_ )
		{
			if ( door.pos.x == playerPos.x + 1 )
			{
				nextDoor = &door;

				break;
			}
		}
	}

	if ( nextDoor )
	{
		nodeManager_->ChangeNode(nextDoor->id);
	}
}

void BaseNode::GetNextDoors()
{
	int32_t i = 0;
	int32_t count = 0;
	for ( auto& chip : mapChip_->GetMapChip()[ mapChip_->GetMapChip().size() - 1 ] )
	{
		if ( chip == ChipIndex::NEXT )
		{
			NextDoor nextDoor;
			nextDoor.pos = { float(i), float(mapChip_->GetMapChip().size() - 1) };
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
}
