#include "BaseEnemy.h"

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
