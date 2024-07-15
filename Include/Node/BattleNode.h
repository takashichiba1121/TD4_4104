#pragma once
#include<BaseNode.h>

class BattleNode :public BaseNode
{
private:
	std::vector<NextDoor> nextdoors_;
public:

	BattleNode() = default;
	~BattleNode() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
	void Finalize()override;
	Vector2 GetPlayerStartPos()override;
};