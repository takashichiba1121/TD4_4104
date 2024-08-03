#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
#include<BossPunchAttack.h>
#include<BossChargeAttack.h>
enum Phase
{
	APPROACH,
	PUNCH,
	CHARGE
};



class BossEnemy :public BaseEnemy
{
private:

	int32_t ATTACK_INTERVAL;

	Vector2 gravity_;
	RectShape* shape_;

	ObjectUserData user;
	Phase phase_;

	RectShape approachHitBox_;
	RectShape playerRect_;

	int32_t attackInterval_;

	std::unique_ptr<BossPunchAttack>punch_;
	std::unique_ptr<BossChargeAttack>charge_;

	int32_t anime_;
	int32_t animeTimer_;
	int32_t animeNum_ = 0;
	int32_t playerDir_;

	
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

	void AnimeUpdate();
};

