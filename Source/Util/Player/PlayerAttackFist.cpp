#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
void PlayerAttackFist::AttackInit(const Vector2& playerPos,bool direction)
{
	if (isAttack_==false)
	{
		isAttack_ = true;

		playerPos_ = playerPos;

		direction_ = direction;
	}
}

void PlayerAttackFist::Attack()
{
	if (isAttack_ )
	{
		AttackTime_++;

		if (AttackTime_> lastAttackTime_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackFist::Draw()
{
	if ( isAttack_ )
	{
		if (direction_ )
		{
			DrawBox(playerPos_.x,playerPos_.y - colisionSize_.y / 2,playerPos_.x + colisionSize_.x,playerPos_.y + colisionSize_.y / 2,GetColor(0,255,0),false);
		}
		else
		{
			DrawBox(playerPos_.x,playerPos_.y - colisionSize_.y / 2,playerPos_.x - colisionSize_.x,playerPos_.y + colisionSize_.y / 2,GetColor(0,255,0),false);
		}
	}
}
