#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
public:
	void AttackInit(const Vector2& playerPos,bool direction) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {return isAttack_;}

private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	const float lastAttackTime_ = 10;

	Vector2 playerPos_;

	bool direction_;

	Vector2 colisionSize_ = { 50,61 };
};

