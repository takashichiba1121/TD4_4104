#pragma once
#include<BaseNode.h>

class BattleNode :public BaseNode
{
	// BaseNode を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

