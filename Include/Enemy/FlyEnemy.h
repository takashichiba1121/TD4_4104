#pragma once

#include "BaseEnemy.h"
class FlyEnemy :public BaseEnemy
{
	Vector2 speed_;
	Vector2 velocity_;
	int64_t attackTime_;
	bool isAttack_;
public:

	void Initialize() override;
	void Update() override;
	void Move();
	void Attack();
	void Draw() override;
};
