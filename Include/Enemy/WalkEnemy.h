#pragma once
#include "BaseEnemy.h"
class WalkEnemy:public BaseEnemy
{
	int32_t speed;
public:

	void Initialize() override;
	void Update() override;
	void Draw() override;
};

