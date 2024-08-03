#include "BossLongRangeAttack.h"
#include <DxlibInclude.h>
#include<GameConfig.h>

BossLongRangeAttack::BossLongRangeAttack()
{
	bullet_ = std::make_unique<BossBullet>();
}

void BossLongRangeAttack::Attack()
{
	if ( !bullet_->IsAttack() )
	{
		isAttack_ = true;
		isPreparation_ = false;
		bullet_->SetDir(dir_);
		bullet_->SetBossPos(bossPos_);
		bullet_->Preparation();
	}
}

void BossLongRangeAttack::Preparation()
{
	isPreparation_ = true;
}

void BossLongRangeAttack::Initialize()
{
	bullet_->Initialize();

	chargeImg_ = LoadGraph(std::string("Resources/Enemy/chargeBoss.png"));
	animeFrame_ = GameConfig::GetBossConfig()->charge.animeFrame;

	int32_t graphSizeX;
	int32_t graphSizeY;
	GetGraphSize(chargeImg_,&graphSizeX,&graphSizeY);

	animeNum_ = graphSizeX / 192;
	drawSize_ = { 192,192 };
}

void BossLongRangeAttack::Update()
{
	if ( isAttack_ )
	{
		AnimeUpdate();

		if ( chargeTime_ == 0 )
		{
			if ( bullet_->IsAttack() )
			{
				freezeTime_ = FREEZE_TIME;
				chargeTime_ = CHARGE_TIME;
				isAttack_ = isShot_ = false;
			}

			if ( !isShot_ && !bullet_->IsAttack() )
			{
				isShot_ = true;
				bullet_->Attack();
				chargeTime_ = -1;
			}
		}
		else if( chargeTime_ != -1 )
		{

			chargeTime_--;
		}

		if ( freezeTime_ == 0 )
		{
			freezeTime_ = -1;
			freezeTime_ = FREEZE_TIME;
			chargeTime_ = CHARGE_TIME;
			isAttack_ = false;
			return;
		}
		else if ( isShot_)
		{
			freezeTime_--;
		}
	}

	if ( !bullet_->IsAttack() )
	{
		isShot_ = false;
	}
}

void BossLongRangeAttack::BulletUpdate()
{
		bullet_->Update();
}

void BossLongRangeAttack::Draw()
{
	if ( isAttack_ )
	{
		DrawRectGraphF(bossPos_.x - drawSize_.x / 2,bossPos_.y - drawSize_.y / 2,0 + drawSize_.x * anime_,0,drawSize_.x,drawSize_.y,chargeImg_,true,dir_ == 1);
	}

	bullet_->Draw();
}

void BossLongRangeAttack::DebugDraw()
{
	bullet_->DebugDraw();
}

void BossLongRangeAttack::SetBossPos(const Vector2& pos)
{
	bossPos_ = pos;
}

void BossLongRangeAttack::SetChargeTime(int32_t time)
{
	chargeTime_ = CHARGE_TIME = time;
}

void BossLongRangeAttack::SetFreezeTime(int32_t time)
{
	freezeTime_ = FREEZE_TIME = time;
}

void BossLongRangeAttack::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossLongRangeAttack::SetBulletTime(int32_t time)
{
	bullet_->SetTime(time);
}

void BossLongRangeAttack::SetBulletSize(const Vector2& size)
{
	bullet_->SetSize(size);
}

void BossLongRangeAttack::SetBulletPower(int32_t power)
{
	bullet_->SetPower(power);
}

void BossLongRangeAttack::SetBulletSpeed(float speed)
{
	bullet_->SetSpeed(speed);
}

bool BossLongRangeAttack::IsAttack() const
{
	return isAttack_;
}

bool BossLongRangeAttack::IsShot() const
{
	return isShot_;
}

void BossLongRangeAttack::AnimeUpdate()
{
	animeTimer_++;

	if ( animeTimer_ == animeFrame_ )
	{
		animeTimer_ = 0;
		anime_++;
		if ( anime_ == animeNum_ )
		{
			anime_ = 0;
		}
	}
}
