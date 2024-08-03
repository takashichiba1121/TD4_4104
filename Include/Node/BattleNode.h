#pragma once
#include<BaseNode.h>
#include <EnemyManager.h>

class BattleNode :public BaseNode
{
private:
	
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