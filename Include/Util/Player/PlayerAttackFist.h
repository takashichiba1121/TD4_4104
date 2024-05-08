#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
	void AttackInit() override;

	void Attack() override;

	void Draw() override;
};

