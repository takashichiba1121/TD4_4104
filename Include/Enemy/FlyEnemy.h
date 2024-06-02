#pragma once

#include "BaseEnemy.h"
class FlyEnemy :public BaseEnemy
{
private:
	int64_t attackTime_;
	int64_t tackleTime_;
	Vector2 targetPos_;
	bool isAttack_;
	const int64_t ATTACK_INTERVAL = 120;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Attack() override;
	void Draw() override;
};