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

	uint32_t GetInterval() override {return interval_;}

	AttackType GetType() override {return type_;}

private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	const float lastAttackTime_ = 10;

	Vector2 attackPos_ = {0,0};

	Vector2 colisionSize_ = { 100,80 };

	const uint32_t interval_ = 30;

	const AttackType type_ = AttackType::Small;
};

