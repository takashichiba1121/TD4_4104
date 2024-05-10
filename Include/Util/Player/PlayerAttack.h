#pragma once
#include"Vector2.h"
class PlayerAttack
{
public:

	virtual void AttackInit(const Vector2& playerPos,bool direction) = 0;

	virtual void Attack() = 0;

	virtual void Draw() = 0;

	virtual bool GetAttack()=0;
};

