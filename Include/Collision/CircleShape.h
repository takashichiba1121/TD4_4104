#pragma once
#include "IShape.h"
#include "Vector2.h"

class CircleShape : public IShape
{
private:

	Vector2 center_;
	float r_;

public:

	virtual ~CircleShape() = default;
	CircleShape();

	const Vector2& GetCenter()const;
	const float GetRadius()const;

	void SetCenter(const Vector2& center);
	void SetRadius(float r);

};

