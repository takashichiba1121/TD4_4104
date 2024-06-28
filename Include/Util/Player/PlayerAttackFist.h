#pragma once
#include "PlayerAttack.h"
class PlayerAttackFist :
    public PlayerAttack
{
private:
	bool isAttack_ = false;

	uint32_t attackType_ = 0;

	uint32_t nextAttack_=0;

	bool isGiveDamage_ = false;

	float AttackTime_ = 0;

	Vector2 DrawPos_;

	Combo combo1 = { 10,{15,-10},{30,30},70,5 };

	Combo combo2 = { 20,{20,-10},{45,30},80,10 };

	Combo combo3 = { 30,{25,-10},{60,30},90,20 };

	float playerPow_;

	RectShape* shape_;

	Vector2* playerPos_;

	bool* direction_;

public:
	void Initialize(Vector2* playerPos,bool* direction) override;

	void AttackInit(float pow) override;

	void Attack() override;

	void Draw() override;

	bool GetAttack() override {return isAttack_;}

	uint32_t GetInterval() override;

	float GetPow() override;

	void OnCollision() override;
};

