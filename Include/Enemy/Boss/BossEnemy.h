#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
#include<BossPunchAttack.h>
#include<BossChargeAttack.h>
#include<BossLongRangeAttack.h>
#include<random>

enum Phase
{
	APPROACH,
	PUNCH,
	CHARGE,
	LONG_RANGE
};



class BossEnemy :public BaseEnemy
{
private:

	int32_t ATTACK_INTERVAL;
	int32_t HP;

	Vector2 gravity_;
	RectShape* shape_;

	ObjectUserData user;
	Phase phase_;
	Phase nextPhase_;

	RectShape attackApproachHitBox_;
	RectShape chargeApproachHitBox_;
	RectShape approachHitBox_;
	RectShape playerRect_;

	int32_t attackInterval_;

	std::unique_ptr<BossPunchAttack>punch_;
	std::unique_ptr<BossChargeAttack>charge_;
	std::unique_ptr<BossLongRangeAttack>longRange_;

	int32_t anime_;
	int32_t animeTimer_;
	int32_t animeNum_ = 0;
	int32_t playerDir_;

	int32_t bodyImg_;

	std::discrete_distribution<int> distribution;
	int32_t probabilities[ 2 ] = {};

	int8_t longRangeAttackCount_ = 0;


public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Draw(Vector2 scrool) override;
	void OnCollision() override;
	~BossEnemy();

private:

	void DebugDraw();

	void ApproachMove();
	void AttackMove();
	void Attack();
	void AnimeUpdate();

	float PlayerDir();

	Phase GetPhase();

};

