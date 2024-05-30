#include "IObject.h"

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

void IObject::Update()
{
	collisionInfo_.object = nullptr;
	collisionInfo_.shape = nullptr;
	collisionInfo_.userData = nullptr;
}
