#pragma once
#include "BaseObject.h"

class BaseEnemy:public BaseObject
{
protected:
	ObjectUserData name_;
	uint32_t id_;

	BaseObject* playerPtr_;
	int32_t attackPower_;
	int32_t defense_;
public:
	virtual void Move() {};
	virtual void Attack() {};
	void SetPos(Vector2 pos);
	void SetVelocity(Vector2 velocity);
	void SetPlayerPtr(BaseObject* ptr);
};

