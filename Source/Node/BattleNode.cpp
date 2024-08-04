#include "BattleNode.h"
#include<CollisionManager.h>
#include<NodeManager.h>
#include<DxlibInclude.h>

void BattleNode::Initialize()
{

}

void BattleNode::Update()
{
	PlayerNodeMove();
	enemys_->Update();
}

void BattleNode::Draw()
{
	enemys_->Draw();
}

void BattleNode::Reset()
{
	{
		int32_t rand = GetRand(3);

		switch ( rand )
		{
		case 0:
			mapChip_->MapLoad("Resources/Export/Map/Map_01.json");
			break;
		case 1:
			mapChip_->MapLoad("Resources/Export/Map/Map_02.json");
			break;
		case 2:
			mapChip_->MapLoad("Resources/Export/Map/Map_03.json");
			break;
		case 3:
			mapChip_->MapLoad("Resources/Export/Map/Map_04.json");
			break;
		default:
			break;
		}
	}


	player_->Reset();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());
}

void BattleNode::Finalize()
{
	for ( auto& door : nextdoors_ )
	{
		mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
	}

	nextdoors_.clear();
	nextDoorsNum_ = 0;
	enemys_->EnemysClear();
}

Vector2 BattleNode::GetPlayerStartPos()
{
	return Vector2();
}
