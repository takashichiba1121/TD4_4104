#pragma once
#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include "ShootEnemy.h"
#include <list>
#include <memory>

class MapChip;
class Player;
enum EnemyType
{
	FLY,
	SHOOT,
	ADJACENT
};
class EnemyManager
{
private:

	static std::list<std::unique_ptr<BaseEnemy>> enemylist_;
	int32_t popTime_;
	const uint32_t POP_INTERVAL = 30;
	const uint32_t MAX_POP_ENEMY_NUM = 15;
	const uint32_t MAX_ENEMY_NUM = 1;
	MapChip* mapchip_;
	static Player* playerPtr_;
	size_t popEnemyCount_;
	size_t deadEnemyCount_;
	BaseEnemy* cursedEnemy_;
	void Pop();//自動ランダム生成
public:
	void SetEnemyPOP(std::string enemyType,Vector2 pos,Vector2 Velocity);
	void SetEnemyPOP(EnemyType enemyType,Vector2 pos);
	void SetPosPop(Vector2 pos);
	//指定生成
	void SetPlayerPtr(Player* playerPtr);
	void Initialize();
	void Update();
	void Draw();
	void SetMapChip(MapChip* mapchip);
	size_t GetEnemyCount();
	bool GameEnd();
	void EnemysClear();
};

