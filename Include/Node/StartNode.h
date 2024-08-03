#pragma once
#include<BaseNode.h>
#include<vector>

class StartNode :public BaseNode
{
private:


public:

	StartNode() = default;
	~StartNode() = default;

	// BaseNode を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
	void Finalize()override;
	Vector2 GetPlayerStartPos()override;
};

