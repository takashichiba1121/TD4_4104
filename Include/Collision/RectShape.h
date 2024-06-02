#pragma once
#include "IShape.h"
#include"Vector2.h"

class RectShape : public IShape
{
private:

	Vector2 center_;

	Vector2 r_;

public:

	virtual ~RectShape() = default;
	RectShape();

	void SetCenter(const Vector2& center);
	void SetRadius(const Vector2& r);

	const Vector2& GetCenter()const;
	const Vector2& GetRadius()const;
};

