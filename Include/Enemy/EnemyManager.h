#pragma once
#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include <list>
#include <memory>

class EnemyManager
{
private:
	static std::list<std::unique_ptr<BaseEnemy>> enemylist_;
	int32_t popTime_;
	const int32_t POP_INTERVAL = 30;
	const int32_t MAX_ENEMY_NUM = 1;
	static BaseObject* playerPtr_;

	void Pop();//自動ランダム生成
public:
	static void SetEnemyPOP(std::string enemyType,Vector2 pos,Vector2 Velocity);//指定生成
	void SetPlayerPtr(BaseObject* playerPtr);
	void Initialize();
	void Update();
	void Draw();
};

