#include "BossLongRangeAttack.h"

void BossLongRangeAttack::Attack()
{
	isAttack_ = true;
}

void BossLongRangeAttack::Initialize()
{
}

void BossLongRangeAttack::Update()
{

	if ( chargeTime_ == 0 )
	{
		if ( !isShot_ )
		{
			isShot_ = true;
		}


		if ( freezeTime_ == 0 )
		{
			freezeTime_ = FREEZE_TIME;
			chargeTime_ = CHARGE_TIME;
			isAttack_ = isShot_ = false;
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
}

void BossLongRangeAttack::SetTime(int32_t time)
{
}

void BossLongRangeAttack::SetDir(int32_t dir)
{
}

bool BossLongRangeAttack::IsAttack() const
{
	return isAttack_;
}
