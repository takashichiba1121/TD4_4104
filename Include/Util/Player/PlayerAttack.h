#pragma once
#include"Vector2.h"
#include<memory>
class PlayerAttack
{
public:

	virtual void AttackInit(const Vector2& playerPos,bool direction) = 0;

	virtual void Attack() = 0;

	virtual void Draw() = 0;

	virtual bool GetAttack()=0;

	virtual uint32_t GetInterval()=0;
};

