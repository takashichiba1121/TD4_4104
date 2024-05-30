#include "RectShape.h"

RectShape::RectShape()
{
	type_ = ShapeType::RECT;
}

void RectShape::SetCenter(const Vector2& center)
{
	center_ = center;
}

void RectShape::SetRadius(const Vector2& r)
{
	r_ = r;
}

const Vector2& RectShape::GetCenter() const
{
	return center_;
}

const Vector2& RectShape::GetRadius() const
{
	return r_;
}
