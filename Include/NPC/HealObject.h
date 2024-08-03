#pragma once
#include "BaseObject.h"
class HealObject:public BaseObject
{
private:
	int32_t healPower;
	RectShape* shape_;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnCollision();
};

