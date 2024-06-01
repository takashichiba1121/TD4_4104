#pragma once
#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include <list>
#include <memory>

class EnemyManager
{
private:
	std::list<std::unique_ptr<BaseEnemy>> enemylist_;
	int32_t popTime_;
	const int32_t POP_INTERVAL = 30;
	const int32_t MAX_ENEMY_NUM = 10;

	void Pop();
public:
	void Initialize();
	void Update();
	void Draw();
};

