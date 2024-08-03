#pragma once
#include<BaseNode.h>
#include "HealObject.h"

class HealingNode:public BaseNode
{
private:
	HealObject healObj_;
public:

	// BaseNode を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
	void Finalize()override;
	Vector2 GetPlayerStartPos()override;
};

