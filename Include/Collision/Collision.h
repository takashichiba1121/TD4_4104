#pragma once
#include"CircleShape.h"
#include"RectShape.h"

class Collision
{
public:

	static bool Circle2Circle(const CircleShape& circleA,const CircleShape& circleB);

	static bool Rect2Rect(const RectShape& rectA,const RectShape& rectB);


};

