#pragma once
#include "BaseObject.h"

class BaseEnemy:public BaseObject
{
protected:
	std::string tag_;
	uint32_t id_;
	int32_t attackPower_;
	int32_t defense_;
	
};

