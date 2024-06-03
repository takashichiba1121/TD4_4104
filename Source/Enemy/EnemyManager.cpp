#include "EnemyManager.h"
#include "DxlibInclude.h"
using namespace std;
std::list<std::unique_ptr<BaseEnemy>> EnemyManager::enemylist_;
BaseObject* EnemyManager::playerptr = nullptr;
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
		if (GetRand(2))
		{
			unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
			temp->Initialize();
			temp->SetPlayerPtr(playerptr);
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			enemylist_.push_back(move(temp));
		}
		else
		{
			unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			enemylist_.push_back(move(temp));
		}
	}
}

void EnemyManager::SetEnemyPOP(std::string enemyType,Vector2 pos,Vector2 Velocity)
{
	if ( enemyType == "Fly" )
	{
		unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
		temp->Initialize();
		temp->SetPlayerPtr(playerptr);
		temp->SetPos(pos);
		temp->SetVelocity(Velocity);
		enemylist_.push_back(move(temp));
	}
	else
	{
		unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetVelocity(Velocity);
		enemylist_.push_back(move(temp));
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
