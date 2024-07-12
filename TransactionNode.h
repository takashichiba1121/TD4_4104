#pragma once
#include "BaseNode.h"
class TransactionNode : public BaseNode
{
	// BaseNode を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

