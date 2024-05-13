#include "BaseEnemy.h"

void BaseEnemy::Damage(int damage)
{
	hp_ -= damage;
}

bool BaseEnemy::IsLive()
{
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
