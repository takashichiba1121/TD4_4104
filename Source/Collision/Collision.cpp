#include "Collision.h"

#include<cmath>

bool Collision::Circle2Circle(const CircleShape& circleA,const CircleShape& circleB)
{
	Vector2 vec = circleB.GetCenter()- circleA.GetCenter();

	float lenge = vec.GetLenge();

	if ( lenge < circleA.GetRadius() + circleB.GetRadius() )
	{
		return false;
	}

	return true;
}

bool Collision::Rect2Rect(const RectShape& rectA,const RectShape& rectB)
{
	if ( rectA.GetCenter().x + rectA.GetRadius().x >= rectB.GetCenter().x - rectB.GetRadius().x &&
		 rectA.GetCenter().x - rectA.GetRadius().x <= rectB.GetCenter().x + rectB.GetRadius().x )
	{
		if ( rectA.GetCenter().y + rectA.GetRadius().y <= rectB.GetCenter().y - rectB.GetRadius().y ||
			rectA.GetCenter().y - rectA.GetRadius().y >= rectB.GetCenter().y + rectB.GetRadius().y )
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}
