#include "CircleShape.h"

CircleShape::CircleShape()
{
	type_ = ShapeType::CIRCLE;
}

const Vector2& CircleShape::GetCenter() const
{
	return center_;
}

const float CircleShape::GetRadius() const
{
	return r_;
}

void CircleShape::SetCenter(const Vector2& center)
{
	center_ = center;
}

void CircleShape::SetRadius(float r)
{
	r_ = r;
}
