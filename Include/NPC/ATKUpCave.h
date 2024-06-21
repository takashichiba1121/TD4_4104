#pragma once
#include "PowerUpCave.h"
class ATKUpCave:public PowerUpCave
{



public:
	void Initialize() override;
	bool StatusChenge() override;
};

