#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	bool isGiveDamage_ = false;

	float AttackTime_ = 0;

	Vector2 DrawPos_;

	const float LAST_ATTACK_TIME_ = 3;

	const Vector2 ATTACK_POS_ = { 15,-10 };

	const Vector2 COLISION_SIZE_ = { 30,30 };

	const uint32_t INTERVAL_ = 10;

	const float POW = 5;

	float playerPow_;

	RectShape* shape_;

public:
	void Initialize() override;

	void AttackInit(const Vector2& playerPos,bool direction,float pow) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {return isAttack_;}

	uint32_t GetInterval() override {return INTERVAL_;}

	float GetPow() override {
		return POW;
	}

	void OnCollision() override;
};

