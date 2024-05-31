#pragma once
#include<forward_list>
#include<vector>

#include"IObject.h"

class CollisionManager
{
private:
	std::forward_list<IObject*>objects_;

	const size_t BLOCK_SIZE = 32;
	std::forward_list<IObject*>mapChipObjects_;
	std::vector<std::vector<uint8_t>>mapChip_;


public:

	void AddObject(IObject* object);
	void RemoveObject(IObject* object);

	void SetMapChip(const std::vector<std::vector<uint8_t>>&mapChip);

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

