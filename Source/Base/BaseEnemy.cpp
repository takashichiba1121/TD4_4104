#include "BaseEnemy.h"
#include <Player.h>

void BaseEnemy::EffectUpdate()
{

	for ( int i = 0; i < effectTimer.size(); i++ )
	{
		Effects ef = static_cast< Effects >( i );
		effectTimer[ i ].CountUp();
		if (IsEffect(ef) && !effectTimer[i].IsCountEnd())
		{
			switch ( ef )
			{
			case BURN:
				if ( effectTimer[ i ].GetCount() % effectDamageInterval[i] == 0)
				{
					
				}
				break;
			case ICED:
				if ( effectTimer[ i ].GetCount() % effectDamageInterval[ i ] == 0 )
				{
					
				}
				break;
			case BIND:
				if ( effectTimer[ i ].GetCount() % effectDamageInterval[ i ] == 0 )
				{
					
				}
				break;
			default:
				break;
			}
		}
		else if ( effectTimer[ i ].IsCountEnd() && !ef == CURSE)
		{
			ReleaseEffect(ef);
		}
	}
	if ( !IsEffect(CURSE) )
	{
		curseStack = 0;
	}
}

void BaseEnemy::Damage(int32_t damage)
{
	if (true)
	{
		if ( IsEffect(ICED) )
		{
			damage = static_cast<int32_t>(static_cast<float>(damage) * 0.25f);
		}
		immortal_ = true;
		hp_ -= damage;
		immortalTime_ = 10;
		if ( IsEffect(CURSE) )
		{
			curseStack++;
		}
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

void BaseEnemy::SetPlayerPtr(Player* ptr)
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
	effectTimer[ effect ].SetEndCount(60);
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

bool BaseEnemy::IsImmortal()
{
	return immortal_;
}

int32_t BaseEnemy::GetImmortalTime()
{
	return immortalTime_;
}

int32_t BaseEnemy::GetCurseStack()
{
	return curseStack;
}
