#pragma once
#include "PlayerAttack.h"
class PlayerAttackWeapon :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	const float LAST_ATTACK_TIME_ = 1;

	Vector2 attackPos_ = { 0,0 };

	Vector2 colisionSize_ = { 200,50 };

	const uint32_t INTERVAL_ = 30;

	const AttackType TYPE_ = AttackType::Big;

	const float Pow = 0;

public:
	void AttackInit(const Vector2& playerPos,bool direction) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {
		return isAttack_;
	}

	uint32_t GetInterval() override {
		return INTERVAL_;
	}

	AttackType GetType() override {
		return TYPE_;
	}

	float GetPow() override {
		return Pow;
	}
};

