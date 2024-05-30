#pragma once
#include "PlayerAttack.h"
class PlayerAttackWeapon :
    public PlayerAttack
{
public:
	void AttackInit(const Vector2& playerPos,bool direction) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {
		return isAttack_;
	}

	uint32_t GetInterval() override {
		return interval;
	}

private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	const float lastAttackTime_ = 1;

	Vector2 attackPos = { 0,0 };

	Vector2 colisionSize_ = { 200,50 };

	const uint32_t interval = 30;
};

