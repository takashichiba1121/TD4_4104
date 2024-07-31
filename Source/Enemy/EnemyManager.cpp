#include "EnemyManager.h"
#include "DxlibInclude.h"
#include "GameConfig.h"
using namespace std;
std::list<std::unique_ptr<BaseEnemy>> EnemyManager::enemylist_;
Player* EnemyManager::playerPtr_ = nullptr;
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
		if (false)
		{
			unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
			temp->Initialize();
			temp->SetPlayerPtr(playerPtr_);
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->SetMapChip(mapchip_);
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
		else if ( true )
		{
			unique_ptr<ShootEnemy> temp = make_unique<ShootEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->SetMapChip(mapchip_);
			temp->SetPlayerPtr(playerPtr_);
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
		else
		{
			unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->SetMapChip(mapchip_);
			temp->SetPlayerPtr(playerPtr_);
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

void EnemyManager::SetPlayerPtr(Player* playerPtr)
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

	int32_t time = -10;
		
	for ( auto& itr : enemylist_ )
	{
		if ( itr->IsImmortal() && itr->IsCursedDamage)
		{
			if ( time < itr->GetImmortalTime() )
			{
				if ( cursedEnemy_ )
				{
					cursedEnemy_->ReleaseEffect(CURSE);
				}
				cursedEnemy_ = itr.get();
				cursedEnemy_->SetEffect(CURSE);
				time = itr->GetImmortalTime();
			}
		}

	}

}

void EnemyManager::Draw()
{
	for ( auto& itr : enemylist_ )
	{
		itr->Draw();
	}
	DrawFormatString(GameConfig::GetWindowWidth() - 200,10,0xffffff,"KillEnemy %d / %d",deadEnemyCount_,MAX_POP_ENEMY_NUM);
}

void EnemyManager::SetMapChip(MapChip* mapchip)
{
	mapchip_ = mapchip;
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
