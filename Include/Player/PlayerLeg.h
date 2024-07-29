#pragma once
#include"Vector2.h"
#include<memory>
class PlayerLeg
{

public:

	uint32_t cost = 0;

public:

	virtual void Initialize(Vector2* playerPos,bool* direction,float* changeAcl) = 0;

	virtual void Move(bool DirBOTTOM,bool isAttack) = 0;

	virtual void JumpStart()=0;

	virtual void Jump()=0;

	virtual void EvasionRoll()=0;

	virtual void Falling()=0;

	virtual void Draw() = 0;

	virtual void Load() = 0;
};

