#include "BossNode.h"

#include<CollisionManager.h>
#include<NodeManager.h>
#include<SceneManager.h>

void BossNode::Initialize()
{
}

void BossNode::Update()
{
	PlayerNodeMove();
	enemys_->Update();

	if ( enemys_->IsEnemyEmpty() )
	{
		for ( auto& door : nextdoors_ )
		{
			mapChip_->MapWrite(door.pos.x,door.pos.y,ChipIndex::NEXT);
		}

		nextdoors_.clear();
		nextDoorsNum_ = 0;
		nodeManager_->End();
	}
}

void BossNode::Draw()
{
	enemys_->Draw();
}

void BossNode::Reset()
{
	mapChip_->MapLoad("Resources/Export/Map/TestBossMap.json");

	player_->Reset();

	GetNextDoors();

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChipPtr());

	enemys_->BossPop();
}

void BossNode::Finalize()
{
	
}

Vector2 BossNode::GetPlayerStartPos()
{
	return Vector2();
}
