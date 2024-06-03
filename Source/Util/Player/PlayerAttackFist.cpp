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

		if (AttackTime_> LAST_ATTACK_TIME_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackFist::Draw()
{
	Vector2 DrawPos;

	if ( direction_ )
	{
		DrawPos = { playerPos_.x+ATTACK_POS_.x + COLISION_SIZE_.x / 2,playerPos_.y +ATTACK_POS_.y };
	}
	else
	{
		DrawPos = { playerPos_.x - ATTACK_POS_.x - COLISION_SIZE_.x / 2,playerPos_.y+ ATTACK_POS_.y };
	}

	if ( isAttack_ )
	{
			DrawBox(DrawPos.x-COLISION_SIZE_.x/2,DrawPos.y - COLISION_SIZE_.y/2,DrawPos.x + COLISION_SIZE_.x/2,DrawPos.y + COLISION_SIZE_.y/2,GetColor(0,255,0),false);
	}
}
