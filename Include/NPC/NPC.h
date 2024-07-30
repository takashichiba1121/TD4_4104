#pragma once
#include "BaseObject.h"
#include <vector>
class NPC:public BaseObject
{
	size_t speechNum_;
	std::vector<std::string> speechs_;
public:
	virtual void Update() override;
	virtual void Damage(int32_t damage)override;
};

