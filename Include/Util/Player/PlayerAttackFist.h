#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	Vector2 playerPos_;

	bool direction_;

	const float LAST_ATTACK_TIME_ = 3;

	const Vector2 ATTACK_POS_ = { 15,0 };

	const Vector2 COLISION_SIZE_ = { 30,60 };

	const uint32_t INTERVAL_ = 10;

	const AttackType TYPE_ = AttackType::Small;

	const float Pow = 5;

public:
	void AttackInit(const Vector2& playerPos,bool direction) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {return isAttack_;}

	uint32_t GetInterval() override {return INTERVAL_;}

	AttackType GetType() override {return TYPE_;}

	float GetPow() override {
		return Pow;
	}
};

