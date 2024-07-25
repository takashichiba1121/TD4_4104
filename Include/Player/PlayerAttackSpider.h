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

	Combo combo1 = { 30,{30,-10},{90,64},70,1,1 };

	Combo combo2 = { 40,{45,-10},{110,64},80,3,2 };

	Combo combo3 = { 50,{60,-10},{130,64},90,5,3 };

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

