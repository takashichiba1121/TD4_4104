#include "EnemyManager.h"
#include "DxlibInclude.h"
#include "GameConfig.h"
#include<BossEnemy.h>
#include "Player.h"

using namespace std;
std::list<std::unique_ptr<BaseEnemy>> EnemyManager::enemylist_;
std::unordered_map<string,int32_t> EnemyManager::texs_;
std::unordered_map<string,int32_t> EnemyManager::sounds_;
Player* EnemyManager::playerPtr_ = nullptr;

void EnemyManager::BossPop()
{
	unique_ptr<BossEnemy> temp = make_unique<BossEnemy>();
	temp->SetPos({ 150,GameConfig::GetWindowHeight() - 192 / 2.0f - 64 });
	temp->SetPlayerPtr(playerPtr_);
	temp->Initialize();
	enemylist_.push_back(move(temp));
}


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
		int32_t rand = GetRand(1000);
		if ( rand <= 200)
		{
			unique_ptr<ShootEnemy> temp = make_unique<ShootEnemy>();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->Initialize();
			temp->SetPlayerPtr(playerPtr_);
			temp->SetMapChip(mapchip_);
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
		else if ( rand <= 550 )
		{
			unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->SetMapChip(mapchip_);
			temp->SetPlayerPtr(playerPtr_);
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
		else
		{
			unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
			temp->Initialize();
			temp->SetPos({ GetRand(850) + 50.f,100.f });
			temp->SetMapChip(mapchip_);
			temp->SetPlayerPtr(playerPtr_);
			temp->SetMovePos();
			enemylist_.push_back(move(temp));
			popEnemyCount_++;
		}
	}
}

void EnemyManager::SetEnemyPop(EnemyType enemyType,Vector2 pos,Vector2 velocity)
{
	if ( enemyType == FLY )
	{
		unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
		temp->SetPos(pos);
		temp->Initialize();
		temp->SetPlayerPtr(playerPtr_);
		temp->SetVelocity(velocity);
		temp->SetMovePos();
		enemylist_.push_back(move(temp));
	}
	else if ( enemyType == SHOOT )
	{
		unique_ptr<ShootEnemy> temp = make_unique<ShootEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetMapChip(mapchip_);
		temp->SetPlayerPtr(playerPtr_);
		temp->SetVelocity(velocity);
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
	else
	{
		unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetVelocity(velocity);
		enemylist_.push_back(move(temp));
	}
}
void EnemyManager::SetEnemyPop(EnemyType enemyType,Vector2 pos)
{
	if ( enemyType == FLY )
	{
		unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetPlayerPtr(playerPtr_);
		temp->SetMapChip(mapchip_);
		temp->SetMovePos();
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
	else if ( enemyType == SHOOT )
	{
		unique_ptr<ShootEnemy> temp = make_unique<ShootEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetMapChip(mapchip_);
		temp->SetPlayerPtr(playerPtr_);
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
	else
	{
		unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetMapChip(mapchip_);
		temp->SetPlayerPtr(playerPtr_);
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
}


void EnemyManager::SetPosPop(Vector2 pos)
{

	int32_t rand = GetRand(1000);
	if ( rand <= 200 )
	{
		unique_ptr<FlyEnemy> temp = make_unique<FlyEnemy>();
		temp->SetPos(pos);
		temp->Initialize();
		temp->SetPlayerPtr(playerPtr_);
		temp->SetMapChip(mapchip_);
		temp->SetMovePos();
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
	else if ( rand <= 550 )
	{
		unique_ptr<ShootEnemy> temp = make_unique<ShootEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetMapChip(mapchip_);
		temp->SetPlayerPtr(playerPtr_);
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
	else
	{
		unique_ptr<WalkEnemy> temp = make_unique<WalkEnemy>();
		temp->Initialize();
		temp->SetPos(pos);
		temp->SetMapChip(mapchip_);
		temp->SetPlayerPtr(playerPtr_);
		enemylist_.push_back(move(temp));
		popEnemyCount_++;
	}
}

void EnemyManager::SetPlayerPtr(Player* playerPtr)
{
	playerPtr_ = playerPtr;
}

void EnemyManager::Update()
{

	screenEnemy_ = 0;
	for ( auto& itr : enemylist_ )
	{
		if ( itr->OnScreen(scroll_) )
		{
			itr->Update();
			screenEnemy_++;
		}
	}

	int32_t cost = 0;
	cost += enemylist_.remove_if([ ](unique_ptr<BaseEnemy>& enemy)
	{
		return enemy->IsLive() == false;
	});
	cost *= 2;
	playerPtr_->AddMaxCost(cost);
	int32_t time = -10;
		
	for ( auto& itr : enemylist_ )
	{
		if ( itr->IsImmortal() && itr->IsCursedDamage())
		{
			if ( time < itr->GetImmortalTime() && itr->IsCursedDamage() )
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
		if ( itr->OnScreen(scroll_) )
		{
			itr->Draw(scroll_);
		}
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

bool EnemyManager::IsEnemyEmpty()
{
	return screenEnemy_ <= 0;
}

void EnemyManager::EnemysClear()
{
	enemylist_.clear();
}

int32_t EnemyManager::GetTexHandle(std::string type)
{
	return texs_[type];
}

int32_t EnemyManager::GetSoundHandle(std::string name)
{
	return sounds_[name];
}

void EnemyManager::TexLoad()
{
	texs_[ "fly" ] = LoadGraph(string("Resources\\Enemy\\enemyFly.png"));

	texs_[ "shootMove" ] = LoadGraph(string("Resources\\Player\\PlayerDush.png"));

	texs_[ "shootAttack" ] = LoadGraph(string("Resources\\Player\\PlayerDush.png"));

	texs_[ "adjacentMove" ] = LoadGraph(string("Resources\\Player\\PlayerDush.png"));

	texs_[ "adjacentAttack" ] = LoadGraph(string("Resources\\Player\\PlayerDush.png"));

	texs_[ "adjacentDash" ] = LoadGraph(string("Resources\\Player\\PlayerDush.png"));


}

void EnemyManager::SoundLoad()
{

	sounds_[ "shootAttack" ] =
		LoadSoundMem(string("Resources\\Sound\\Enemy\\SFX_enemy_archer_arrowRelease.mp3"));
	sounds_[ "shootBeforeAttack" ] =
		LoadSoundMem(string("Resources\\Sound\\Enemy\\SFX_enemy_archer_DrawingBow.mp3"));
	sounds_[ "flyBeforeAttack" ] =
		LoadSoundMem(string("Resources\\Sound\\Enemy\\SFX_enemy_fly_DetectPlayer.mp3"));
	sounds_[ "freeze" ] =
		LoadSoundMem(string("Resources\\Sound\\Enemy\\SFX_enemy_freeze.mp3"));
	sounds_[ "meleeAttack" ] =
		LoadSoundMem(string("Resources\\Sound\\Enemy\\SFX_enemy_melee_Attack.mp3"));
}

void EnemyManager::Finalize()
{
	enemylist_.clear();
}

void EnemyManager::SetScroll(Vector2 scroll)
{
	scroll_ = scroll;
}
