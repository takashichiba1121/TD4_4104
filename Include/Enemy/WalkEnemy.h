#pragma once
#include "BaseEnemy.h"
class WalkEnemy:public BaseEnemy
{
	int32_t speed_;
	Vector2 velocity_;
	Vector2 gravity_;
public:

	void Initialize() override;
	void Update() override;
	void Move();
	void Attack();
	void Draw() override;
};

