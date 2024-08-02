#include "BossLongRangeAttack.h"

BossLongRangeAttack::BossLongRangeAttack()
{
	bullet_ = std::make_unique<BossBullet>();
}

void BossLongRangeAttack::Attack()
{
	isAttack_ = true;
}

void BossLongRangeAttack::Initialize()
{
	bullet_->Initialize();
}

void BossLongRangeAttack::Update()
{
	if ( chargeTime_ == 0 )
	{
		if ( bullet_->IsAttack())
		{
			freezeTime_ = FREEZE_TIME;
			chargeTime_ = CHARGE_TIME;
			isAttack_ = isShot_ = false;
		}

		if ( !isShot_ &&!bullet_->IsAttack())
		{
			isShot_ = true;
			bullet_->SetDir(dir_);
			bullet_->SetBossPos(bossPos_);
			bullet_->Attack();
			chargeTime_ = -1;
		}
	}
	else
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
	else if ( isShot_ && bullet_->IsAttack() )
	{
		freezeTime_--;
	}

	if ( freezeTime_ == -1 && chargeTime_ == -1 )
	{
		freezeTime_ = FREEZE_TIME;
		chargeTime_ = CHARGE_TIME;
		isAttack_ = false;
	}

	if ( !bullet_->IsAttack() )
	{
		isShot_ = false;
	}

}

void BossLongRangeAttack::BulletUpdate()
{
	if ( isShot_ )
	{
		bullet_->Update();
	}
}

void BossLongRangeAttack::Draw()
{
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
	chargeTime_ = CHARGE_TIME + time;
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
