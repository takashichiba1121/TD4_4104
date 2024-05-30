#pragma once
#include<forward_list>
#include<memory>

#include"IObject.h"

class CollisionManager
{
private:
	std::forward_list<IObject*>objects_;

public:

	void AddObject(IObject* object);
	void RemoveObject(IObject* object);

	void Update();

private:

	bool CheckCollisionPair(IObject* objectA,IObject* objectB);

};

