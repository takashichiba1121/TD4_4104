#pragma once
#include<BaseNode.h>
#include <EnemyManager.h>

class BattleNode :public BaseNode
{
private:
	std::unique_ptr<EnemyManager> enemys_;
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