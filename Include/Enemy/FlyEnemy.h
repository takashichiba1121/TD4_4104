#pragma once

#include "BaseEnemy.h"
class FlyEnemy :public BaseEnemy
{
	Vector2 speed;
	Vector2 velocity;
	int64_t attackTime;
public:

	void Initialize() override;
	void Update() override;
	void Move();
	void Attack();
	void Draw() override;
};
