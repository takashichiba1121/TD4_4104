#include "PlayerAttackFist.h"

void PlayerAttackFist::AttackInit()
{
	isAttack_=true;
}

void PlayerAttackFist::Attack()
{
	if (isAttack_ )
	{
		AttackTime_++;

		if (AttackTime_>30 )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackFist::Draw()
{
	
}
