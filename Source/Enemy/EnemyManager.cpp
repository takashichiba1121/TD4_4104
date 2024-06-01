#include "EnemyManager.h"
#include "DxlibInclude.h"
using namespace std;

void EnemyManager::Initialize()
{
	popTime_ = POP_INTERVAL;
}

void EnemyManager::Pop()
{
	popTime_--;
	if ( popTime_ <= 0 )
	{
		if ( enemylist_.size() >= MAX_ENEMY_NUM ) return;
		popTime_ = POP_INTERVAL;
		if ( false)//FlyEnemyが未完成なので一時的に封印
		{
			unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(450) + 50.f,100.f });
			enemylist_.push_back(move(temp));
		}
		else
		{
			unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(450) + 50.f,100.f });
			enemylist_.push_back(move(temp));
		}
	}
}

void EnemyManager::Update()
{
	Pop();
	for ( auto& itr : enemylist_ )
	{
		itr->Update();
	}
}

void EnemyManager::Draw()
{
	for ( auto& itr : enemylist_ )
	{
		itr->Draw();
	}
}
