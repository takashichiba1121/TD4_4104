#pragma once
#include "BaseNode.h"
class TransactionNode : public BaseNode
{
private:
public:

	// BaseNode を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
	void Finalize()override;
	Vector2 GetPlayerStartPos()override;
};

