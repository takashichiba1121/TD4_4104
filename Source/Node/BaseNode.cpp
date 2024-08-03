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

void BaseNode::PlayerNodeMove()
{
	Vector2 playerPos = mapChip_->GetPos(player_->GetPos().x,player_->GetPos().y);
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
