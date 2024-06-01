#pragma once
#include "BaseEnemy.h"

class WalkEnemy:public BaseEnemy
{
private:
	Vector2 gravity_;
public:

	void Initialize() override;
	void Update() override;
	void Move();
	void Attack();
	void Draw() override;
};

