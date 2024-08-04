#include "BaseEnemy.h"
#include <Player.h>
#include "GameConfig.h"

void BaseEnemy::EffectUpdate()
{

	for ( size_t i = 0; i < effectTimer.size(); i++ )
	{
		Effects ef = static_cast< Effects >( i );
		effectTimer[ i ].CountUp();
		if (IsEffect(ef) && !effectTimer[i].IsCountEnd())
		{
			switch ( ef )
			{
			case BURN:
				if ( static_cast<int32_t>(effectTimer[ i ].GetCount())% effectDamageInterval[i] == 0)
				{
					Damage(effectDamage[ ef ],BURN,true);
				}
				break;
			case ICED:
				if ( static_cast<int32_t>(effectTimer[ i ].GetCount())% effectDamageInterval[ i ] == 0 )
				{
					Damage(effectDamage[ ef ],ICED,true);
				}
				break;
			case BIND:
				if (static_cast<int32_t>( effectTimer[ i ].GetCount()) % effectDamageInterval[ i ] == 0 )
				{
					Damage(effectDamage[ ef ],BIND,true);
				}
				break;
			default:
				break;
			}
		}
		else if ( effectTimer[ i ].IsCountEnd() && ef != CURSE)
		{
			ReleaseEffect(ef);
		}
	}
	if ( !IsEffect(CURSE) )
	{
		curseStack = 0;
	}
}

void BaseEnemy::Damage(int32_t damage,Effects effect,bool effectD)
{
	if (!immortal_)
	{
		SetEffect(effect);
		switch ( effect )
		{
		case BIND:
			effectDamage[ effect ] = damage * 0.15f;
			break;
		case ICED:
			effectDamage[ effect ] = damage * 0.50f;
			break;
		case BURN:
			effectDamage[ effect ] = damage * 0.25f;
			break;
		default:
			break;
		}
		if ( IsEffect(ICED) )
		{
			damage = static_cast<int32_t>(static_cast<float>(damage) * 0.25f);
		}
		immortal_ = true;
		hp_ -= damage;
		immortalTime_ = 10;
		if ( playerPtr_ )
		{
			if ( playerPtr_->GetEyeTag() == PlayerEyeTags::Curse )
			{
				isCursedDamage_ = true;
			}
		}
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
	if ( effect >= END )
	{
		return;
	}
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

bool BaseEnemy::IsCursedDamage()
{
	return isCursedDamage_;
}

bool BaseEnemy::IsImmortal()
{
	return immortal_;
}

bool BaseEnemy::OnScreen(Vector2 scrool)
{
	if (pos_.x + scrool.x >= -32 && pos_.x + scrool.x <= GameConfig::GetWindowWidth()+32)
	{
		if (pos_.y + scrool.y >= -32 && pos_.y + scrool.y <= GameConfig::GetWindowHeight()+32)
		{
			return true;
		}
	}
	return false;
}

int32_t BaseEnemy::GetImmortalTime()
{
	return immortalTime_;
}

int32_t BaseEnemy::GetCurseStack()
{
	return curseStack;
}

void BaseEnemy::AnimeUpdate(bool loop)
{
	animeTimer_++;

	if ( animeTimer_ == animeSpeed_ )
	{
		animeTimer_ = 0;
		anime_++;

		if ( anime_ == animeNum_ && loop)
		{
			anime_ = 0;
		}

		anime_ = min(animeNum_-1,anime_);
	}
}
