#include "IObject.h"
#include"CollisionManager.h"

IObject::~IObject()
{
	shape_.release();
	shape_.reset(nullptr);
	CollisionManager::GetInstance()->RemoveObject(this);
}

void IObject::OnCollision()
{
}

void IObject::SetShape(IShape* shape)
{
	shape_.reset(shape);
}

void IObject::SetCollisionAttribute(uint32_t collisionAttribute)
{
	collisionAttribute_ = collisionAttribute;
}

void IObject::SetCollisionMask(uint32_t collisionMask)
{
	collisionMask_ = collisionMask;
}

uint32_t IObject::GetCollisionAttribute()const
{
	return collisionAttribute_;
}

uint32_t IObject::GetCollisionMask()const
{
	return collisionMask_;
}

ShapeType IObject::GetShapeType()const
{
	return shape_->GetShapeType();
}

IShape* IObject::GetShape() const
{
	return shape_.get();
}

const CollisionInfo& IObject::GetCollisionInfo() const
{
	return collisionInfo_;
}

bool IObject::IsMapChipObject() const
{
	return mapChipObject_;
}

void IObject::SetMapChipCenter(Vector2* center)
{
	center_ = center;
	mapChipObject_ = true;
}

void IObject::SetMapChipRadius(const Vector2& r)
{
	r_ = r;
}

const Vector2& IObject::GetMapChipRadius() const
{
	return r_;
}

void IObject::SetMapChipSpeed(const Vector2& speed)
{
	speed_ = speed;
}

const Vector2& IObject::GetMapChipSpeed() const
{
	return speed_;
}

uint8_t IObject::GetOnDir()
{
	return dir_;
}

void IObject::MapChipObjectEnable()
{
	isMapChipCollision_ = true;

}

void IObject::MapChipObjectDisable()
{
	isMapChipCollision_ = false;
}

void IObject::CollisionEnable()
{
	isCollision_ = true;
}

void IObject::CollisionDisable()
{
	isCollision_ = false;
	dir_ = 0;
}

void IObject::Update()
{
	collisionInfo_.object = nullptr;
	collisionInfo_.shape = nullptr;
	collisionInfo_.userData = nullptr;
}
