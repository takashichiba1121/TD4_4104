#pragma once
#include "PlayerAttack.h"
class PlayerAttackSpider :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	uint32_t attackType_ = 0;

	uint32_t nextAttack_ = 0;

	bool isGiveDamage_ = false;

	float AttackTime_ = 0;

	Vector2 DrawPos_;

	float AttackInterval_ = 0;

	const float LAST_ATTACK_TIME_ = 10;

	const Vector2 ATTACK_POS_ = {30,0 };

	const Vector2 COLISION_SIZE_ = { 64,64 };

	const uint32_t INTERVAL_ = 10;

	const float POW_ = 60;

	const float MOVE_ = 3;

	float playerPow_;

	float  playerCrit_;

	float  playerCdmg_;

	RectShape* shape_;

	Vector2* playerPos_;

	bool* direction_;

	Vector2* velocity_;

public:

	static const uint32_t ArmCost = 0;

public:
	void Initialize(Vector2* playerPos,Vector2* velocity,bool* direction) override;

	void AttackInit(float pow,float changeCrit_,float changeCdmg_) override;

	void Attack() override;

	void Draw(Vector2 scroll) override;

	bool IsAttack() override {
		return isAttack_;
	}

	void OnCollision() override;
};

