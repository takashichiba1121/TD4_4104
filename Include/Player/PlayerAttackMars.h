#pragma once
#include "PlayerAttack.h"
class PlayerAttackMars :
	public PlayerAttack
{
private:
	bool isAttack_ = false;

	bool isGiveDamage_ = false;

	float AttackTime_ = 0;

	float AttackInterval_ = 0;

	Vector2 DrawPos_;

	const float LAST_ATTACK_TIME_=10;

	const Vector2 ATTACK_POS_ = {15,-10};

	const Vector2 COLISION_SIZE_ = {64,64};

	const uint32_t INTERVAL_=60;

	const float POW_=100;

	const float MOVE_=5;

	float playerPow_;

	RectShape* shape_;

	Vector2* playerPos_;

	bool* direction_;

	Vector2* velocity_;

public:

	static const uint32_t ArmCost = 0;

public:
	void Initialize(Vector2* playerPos,Vector2* velocity,bool* direction) override;

	void AttackInit(float pow) override;

	void Attack() override;

	void Draw() override;

	bool IsAttack() override {
		return isAttack_;
	}

	void OnCollision() override;
};

