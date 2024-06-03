#pragma once
#include "BaseObject.h"

class BaseEnemy:public BaseObject
{
protected:
	std::string tag_;
	uint32_t id_;

	BaseObject* playerPosPtr_;
	int32_t attackPower_;
	int32_t defense_;
public:
	virtual void Move() {};
	virtual void Attack() {};
};

