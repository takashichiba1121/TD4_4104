#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
void PlayerAttackFist::AttackInit(const Vector2& playerPos,bool direction)
{
	if (isAttack_==false)
	{
		isAttack_ = true;

		if ( direction )
		{
			attackPos = {playerPos.x + colisionSize_.x/2,playerPos.y};
		}
		else
		{
			attackPos = { playerPos.x - colisionSize_.x/2,playerPos.y };
		}
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
			DrawBox(attackPos.x-colisionSize_.x/2,attackPos.y - colisionSize_.y/2,attackPos.x + colisionSize_.x/2,attackPos.y + colisionSize_.y/2,GetColor(0,255,0),false);
	}
}
