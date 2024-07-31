#pragma once
#include"Vector2.h"
#include<memory>
#include"IObject.h"
#include"RectShape.h"
class PlayerAttack:public IObject
{
	

public:
	uint32_t cost = 0;

public:

	virtual void Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)=0;

	virtual void AttackInit(float pow,float changeCrit_,float changeCdmg_) = 0;

	virtual void Attack() = 0;

	virtual void Draw() = 0;

	virtual	bool IsAttack()=0;
};

