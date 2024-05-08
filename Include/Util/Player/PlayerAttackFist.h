#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
public:
	void AttackInit() override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {return isAttack_;}

private:
	bool isAttack_ = false;

	float AttackTime_ = 0;
};

