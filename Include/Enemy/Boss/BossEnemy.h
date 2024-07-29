#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
#include<BossPunchAttack.h>

enum Phase
{
	APPROACH,
	ATTACK,
};



class BossEnemy :public BaseEnemy
{
private:
	Vector2 gravity_;
	RectShape* shape_;

	ObjectUserData user;
	Phase phase_;

	RectShape approachHitBox_;
	RectShape playerRect_;

	int32_t attackInterval_;

	std::unique_ptr<BossPunchAttack>attack_;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Draw() override;
	void OnCollision() override;

private:

	void DebugDraw();

	void ApproachMove();
	void AttackMove();
	void Attack();

	float PlayerDir();
};

