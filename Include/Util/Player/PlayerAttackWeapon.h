#pragma once
#include "PlayerAttack.h"
#include"RectShape.h"
class PlayerAttackWeapon :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	float AttackTime_ = 0;

	const float LAST_ATTACK_TIME_ = 1;

	Vector2 ATTACK_POS_ = { 0,0 };

	Vector2 COLISION_SIZE_ = { 200,50 };

	const uint32_t INTERVAL_ = 30;

	const float POW = 0;

	float playerPow;

	Vector2 DrawPos_;

	RectShape* shape_;

public:
	void Initialize() override;

	void AttackInit(const Vector2& playerPos,bool direction,float pow) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {
		return isAttack_;
	}

	uint32_t GetInterval() override {
		return INTERVAL_;
	}

	float GetPow() override {
		return POW;
	}
};

