#include "BaseEnemy.h"

void BaseEnemy::Damage(int damage)
{
	hp_ -= damage;
}

bool BaseEnemy::IsLive()
{
	if ( hp_ <= 0 )
	{
		islive_ = false;
	}
	return islive_;
}

Vector2 BaseEnemy::GetHitBoxSize()
{
	return hitboxSize_;
}

Vector2 BaseEnemy::GetDrawSize()
{
	return drawSize_;
}

int BaseEnemy::GetHp()
{
	return hp_;
}

void BaseEnemy::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void BaseEnemy::SetPlayer(IObject* playerPtr)
{
	playerPosPtr_ = playerPtr;
}
