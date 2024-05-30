#pragma once
#include<stdint.h>

enum ShapeType
{
	CIRCLE,
	RECT,

	SHAPE_TYPE_NUM
};

class IShape
{
protected:

	ShapeType type_;

public:

	ShapeType GetShapeType();

	virtual ~IShape() =default;
};