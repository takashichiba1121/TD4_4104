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
	const uint32_t MAX_ENEMY_NUM = 5;
	MapChip* mapchip_;
	static Player* playerPtr_;
	size_t popEnemyCount_;
	size_t deadEnemyCount_;
	BaseEnemy* cursedEnemy_;
	size_t screenEnemy_ = 0;
	Vector2 scroll_;

	static std::array<int32_t,3> texs_;

	void Pop();
	//自動ランダム生成

	void BossPop();
public:
	void SetEnemyPop(EnemyType enemyType,Vector2 pos,Vector2 Velocity);
	//指定生成
	void SetEnemyPop(EnemyType enemyType,Vector2 pos);
	//Pos指定型ランダム
	void SetPosPop(Vector2 pos);
	void SetPlayerPtr(Player* playerPtr);
	void Initialize();
	void Update();
	void Draw();
	void SetMapChip(MapChip* mapchip);
	size_t GetEnemyCount();
	bool GameEnd();
	bool IsEnemyEmpty();
	void EnemysClear();
	static int32_t GetTexHandle(EnemyType type);
	static void TexLoad();
	static void Finalize();
	void SetScroll(Vector2 scroll);
};

