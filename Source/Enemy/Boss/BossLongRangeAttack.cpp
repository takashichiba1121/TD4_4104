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
		if ( !isShot_ &&!bullet_->IsAttack())
		{
			isShot_ = true;
			bullet_->SetDir(dir_);
			bullet_->SetBossPos(bossPos_);
			bullet_->Attack();
		}
		else if( bullet_->IsAttack() )
		{
			freezeTime_ = FREEZE_TIME;
			chargeTime_ = CHARGE_TIME;
			isAttack_ = isShot_ = false;
			return;
		}


		if ( freezeTime_ == 0 )
		{
			freezeTime_ = FREEZE_TIME;
			chargeTime_ = CHARGE_TIME;
			isAttack_ = isShot_ = false;
			return;
		}
		else
		{
			freezeTime_--;
		}
	}
	else
	{
		chargeTime_--;
	}
}

void BossLongRangeAttack::Draw()
{
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
