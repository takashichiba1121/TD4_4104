#pragma once
#include "PlayerAttack.h"
#include"RectShape.h"
class PlayerAttackGun :
    public PlayerAttack
{
private:
	uint32_t isAttack_ = false;

	float AttackTime_ = 0;

	const float LAST_ATTACK_TIME_ = 20;

	const Vector2 ATTACK_POS_ = { 30,0 };

	const Vector2 COLISION_SIZE_ = { 64,32 };

	const uint32_t INTERVAL_ = 30;

	const float POW_ = 90;

	const float MOVE_ = 0;

	Vector2 DrawPos_;

	RectShape* shape_;

	Vector2* playerPos_;

	bool* direction_;

	Vector2* velocity_;

	int textureId_;

public:

	static const uint32_t ArmCost = 15;

public:
	void Initialize(Vector2* playerPos,Vector2* velocity,bool* direction) override;

	void AttackInit(float pow,float changeCrit_,float changeCdmg_) override;

	void Attack() override;

	void Draw(Vector2 scroll) override;

	bool IsAttack() override {
		return isAttack_;
	}
};

