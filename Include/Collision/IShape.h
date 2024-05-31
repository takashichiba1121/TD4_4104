#pragma once
#include<stdint.h>

enum ShapeType
{
	S_CIRCLE,
	S_RECT,

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