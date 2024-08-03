#pragma once
#include<forward_list>
#include<vector>

#include"IObject.h"
#include<CollisionConfig.h>

class CollisionManager
{
private:
	std::forward_list<IObject*>objects_;

	const size_t BLOCK_SIZE = 64;
	std::forward_list<IObject*>mapChipObjects_;
	std::vector<std::vector<uint8_t>>* mapChip_;
	Vector2 screenPos_;

public:

	void AddObject(IObject* object);
	void RemoveObject(IObject* object);

	void SetMapChip(std::vector<std::vector<uint8_t>>* mapChip);

	void SetScreenPos(const Vector2& screenPos);

	void Update();

	static CollisionManager* GetInstance();

private:

	bool CheckCollisionPair(IObject* objectA,IObject* objectB);

	bool DownCollision(IObject* object);
	bool TopCollision(IObject* object);
	bool LeftCollision(IObject* object);
	bool RightCollision(IObject* object);


	CollisionManager() = default;
	~CollisionManager() = default;
};

