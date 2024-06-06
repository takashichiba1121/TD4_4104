#pragma once
#include "BaseObject.h"

class BaseEnemy:public BaseObject
{
protected:
	ObjectUserData name_;
	uint32_t id_;
	bool immortal_ = false;
	BaseObject* playerPtr_;
	int32_t attackPower_;
	int32_t defense_;
	int32_t immotalTime_;
public:
	virtual void Move() {};
	virtual void Attack() {};
	virtual void Damage(int32_t damage) override;
	void SetPos(Vector2 pos);
	void SetVelocity(Vector2 velocity);
	void SetPlayerPtr(BaseObject* ptr);
};

