#pragma once
#include "BaseEnemy.h"

class WalkEnemy:public BaseEnemy
{
	int32_t speed;
	Vector2 velocity;
	Vector2 gravity;
public:

	void Initialize() override;
	void Update() override;
	void Draw() override;
};

