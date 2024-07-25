#include "BaseEnemy.h"

void BaseEnemy::EffectUpdate()
{
}

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

void BaseEnemy::SetMapChip(MapChip* mapptr)
{
	mapchip_ = mapptr;
}

void BaseEnemy::SetEffect(Effects effect)
{
	int8_t effectBit = 0b1 << effect;
	statusEffects_ |= effectBit;
}

void BaseEnemy::ReleaseEffect(Effects effect)
{
	int8_t effectBit = 0b1 << effect;
	statusEffects_ &= ~effectBit;
}

bool BaseEnemy::IsEffect(Effects effect)
{
	int8_t judge = statusEffects_;
	judge = judge >> effect;
	if ( judge & 0b1 )
	{
		return true;
	}
	return false;
}
