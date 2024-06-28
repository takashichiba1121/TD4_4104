#pragma once
#include"Vector2.h"
#include<memory>
#include"IObject.h"
#include"RectShape.h"
class PlayerAttack:public IObject
{
public:
	struct Combo{
		const float LAST_ATTACK_TIME_;

		const Vector2 ATTACK_POS_;

		const Vector2 COLISION_SIZE_;

		const uint32_t INTERVAL_;

		const float POW;
	};

public:

	virtual void Initialize(Vector2* playerPos,bool* direction)=0;

	virtual void AttackInit(float pow) = 0;

	virtual void Attack() = 0;

	virtual void Draw() = 0;

	virtual	bool GetAttack()=0;

	virtual uint32_t GetInterval()=0;

	virtual float GetPow()=0;
};

