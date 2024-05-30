#pragma once
#include<memory>
#include"IShape.h"

struct CollisionInfo
{
	IObject* object = nullptr;
	IShape* shape = nullptr;
	void* userData = nullptr;
};


class IObject 
{
private:

	friend class CollisionManager;

	std::unique_ptr<IShape>shape_;

	uint32_t collisionAttribute_ = 0xffffffff;

	uint32_t collisionMask_ = 0xffffffff;

	CollisionInfo collisionInfo_;

protected:

	void* userData_;

public:

	virtual void OnCollision();


	void SetShape(IShape* shape);

	void SetCollisionAttribute(uint32_t collisionAttribute);
	void SetCollisionMask(uint32_t collisionMask);

	uint32_t GetCollisionAttribute()const;
	uint32_t GetCollisionMask()const;

	ShapeType GetShapeType()const;
	IShape* GetShape()const;

	const CollisionInfo& GetCollisionInfo()const;

private:

	void Update();
};

