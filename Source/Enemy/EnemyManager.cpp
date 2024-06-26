#include "EnemyManager.h"
#include "DxlibInclude.h"
#include "GameConfig.h"
using namespace std;
std::list<std::unique_ptr<BaseEnemy>> EnemyManager::enemylist_;
BaseObject* EnemyManager::playerPtr_ = nullptr;
void EnemyManager::Initialize()
{
	enemylist_.clear();
	popTime_ = POP_INTERVAL;
}

void EnemyManager::Pop()
{
	popTime_--;
	if ( popTime_ <= 0 )
	{
		if ( enemylist_.size() >= MAX_ENEMY_NUM || popEnemyCount_ >= MAX_POP_ENEMY_NUM) return;
		popTime_ = POP_INTERVAL;
		if (GetRand(2) - 1)
		{
			unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
			temp->Initialize();
			temp->SetPlayerPtr(playerPtr_);
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
		else
		{
			unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
	}
}

void EnemyManager::SetEnemyPOP(std::string enemyType,Vector2 pos,Vector2 Velocity)
{
	if ( enemyType == "Fly" )
	{
		unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
		temp->Initialize();
		temp->SetPlayerPtr(playerPtr_);
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

void EnemyManager::SetPlayerPtr(BaseObject* playerPtr)
{
	playerPtr_ = playerPtr;
}

void EnemyManager::Update()
{
	Pop();
	for ( auto& itr : enemylist_ )
	{
		itr->Update();
	}

	deadEnemyCount_ += enemylist_.remove_if([](unique_ptr<BaseEnemy>& enemy )
	{
		return enemy->IsLive() == false;
	});
}

void EnemyManager::Draw()
{
	for ( auto& itr : enemylist_ )
	{
		itr->Draw();
	}
	DrawFormatString(GameConfig::GetWindowWidth() - 200,10,0xffffff,"KillEnemy %d / %d",deadEnemyCount_,MAX_POP_ENEMY_NUM);
}

size_t EnemyManager::GetEnemyCount()
{
	return enemylist_.size();
}

bool EnemyManager::GameEnd()
{
	if ( enemylist_.empty() && popEnemyCount_ >= MAX_POP_ENEMY_NUM )
	{
		return true;
	}
	return false;
}

void EnemyManager::EnemysClear()
{
	enemylist_.clear();
}
