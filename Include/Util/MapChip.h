#pragma once
#include<vector>
#include<string>

#include"Vector2.h"
#include<Player.h>
#include<EnemyManager.h>

class MapChip
{
private:

	const int32_t BLOCK_SIZE = 64;

	std::vector < std::vector<uint8_t>>map_;

	int32_t roadChipHandle_ = 0;
	int32_t wallChipHandle_ = 0;
	Vector2 screenPos_;
	Player* player_;
	EnemyManager* enemyManager_;

public:

	void Initialize();

	void MapLoad(const std::string& path);

	void MapWrite(int32_t x,int32_t y,uint8_t index);

	const std::vector < std::vector<uint8_t>>& GetMapChip()const;
	std::vector < std::vector<uint8_t>>* GetMapChipPtr();

	uint8_t GetPosElement(int32_t x,int32_t y)const;
	Vector2 GetPos(int32_t x,int32_t y)const;
	uint8_t GetNumOfArrayElement(int32_t x,int32_t y)const;
	const Vector2& GetScreenPos()const;

	void SetPlayer(Player* player);
	void SetEnemyManager(EnemyManager* enemyManager);



	void Draw(const Vector2& screenPos);

	void RoomInstallation(const std::string& directoryPath,const Vector2& leftTop);
};
