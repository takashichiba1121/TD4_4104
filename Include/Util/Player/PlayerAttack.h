#pragma once
#include"Vector2.h"
#include<memory>
#include"IObject.h"
#include"RectShape.h"
class PlayerAttack:public IObject
{
public:

	virtual void Initialize()=0;

	virtual void AttackInit(const Vector2& playerPos,bool direction,float pow) = 0;

	virtual void Attack() = 0;

	virtual void Draw() = 0;

	virtual bool GetAttack()=0;

	virtual uint32_t GetInterval()=0;

	virtual float GetPow()=0;
};

