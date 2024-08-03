#pragma once
#include<memory>
#include"IShape.h"
#include<Vector2.h>

#include"ChipIndex.h"

enum OnDir
{
	LEFT,
	RIGHT,
	UP,
	BOTTOM
};

class IObject;

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

	bool isCollision_ = true;
	bool mapChipObject_ = false;
	Vector2* center_;
	Vector2 r_;
	Vector2 speed_;
	uint8_t dir_;
protected:

	void* userData_;

public:

	IObject() = default;
	virtual ~IObject();

	virtual void OnCollision();


	void SetShape(IShape* shape);

	void SetCollisionAttribute(uint32_t collisionAttribute);
	void SetCollisionMask(uint32_t collisionMask);

	uint32_t GetCollisionAttribute()const;
	uint32_t GetCollisionMask()const;

	ShapeType GetShapeType()const;
	IShape* GetShape()const;

	const CollisionInfo& GetCollisionInfo()const;

	bool IsMapChipObject() const;

	void SetMapChipCenter(Vector2* center);

	void SetMapChipRadius(const Vector2& r);
	const Vector2& GetMapChipRadius()const;

	void SetMapChipSpeed(const Vector2& r);
	const Vector2& GetMapChipSpeed()const;

	uint8_t GetOnDir();

	void MapChipObjectEnable();
	void MapChipObjectDisable();

	void CollisionEnable();
	void CollisionDisable();

private:

	void Update();
};

