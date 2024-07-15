#pragma once
#include<BaseNode.h>
#include<vector>

class StartNode :public BaseNode
{
private:

	std::vector<NextDoor> nextdoors_;

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

