#include "BaseEnemy.h"

void BaseEnemy::Damage(int32_t damage)
{
	if (true)
	{
		immortal_ = true;
		hp_ -= damage;
		immotalTime_ = 10;
	}
}

void BaseEnemy::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void BaseEnemy::SetVelocity(Vector2 velocity)
{
	velocity_ = velocity;
}

void BaseEnemy::SetPlayerPtr(BaseObject* ptr)
{
	playerPtr_ = ptr;
}
