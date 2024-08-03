#include "Collision.h"

#include<cmath>

float DistanceSqrf(const float t_x1,const float t_y1,const float t_x2,const float t_y2)
{
	float dx = t_x2 - t_x1;
	float dy = t_y2 - t_y1;

	return ( dx * dx ) + ( dy * dy );
}

bool Collision::Circle2Circle(const CircleShape& circleA,const CircleShape& circleB)
{
	Vector2 vec = circleB.GetCenter()- circleA.GetCenter();

	float lenge = vec.GetLenge();

	if ( lenge > circleA.GetRadius() + circleB.GetRadius() )
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

bool Collision::Rect2Circe(const RectShape& rect,const CircleShape& circle)
{
	bool nResult = false;

	// 四角形の四辺に対して円の半径分だけ足したとき円が重なっていたら
	if ( ( circle.GetCenter().x > rect.GetCenter().x - rect.GetRadius().x - circle.GetRadius() ) &&
		( circle.GetCenter().x < rect.GetCenter().x + rect.GetRadius().x + circle.GetRadius() ) &&
		( circle.GetCenter().y > rect.GetCenter().y - rect.GetRadius().y - circle.GetRadius() ) &&
		( circle.GetCenter().y < rect.GetCenter().y + rect.GetRadius().y + circle.GetRadius() ) )
	{
		nResult = true;
		float fl = circle.GetRadius() * circle.GetRadius();


		// 左
		if ( circle.GetCenter().x < rect.GetCenter().x - rect.GetRadius().x )
		{
			// 左上
			if ( ( circle.GetCenter().y < rect.GetCenter().y - rect.GetRadius().y ) )
			{
				if ( ( DistanceSqrf(rect.GetCenter().x - rect.GetRadius().x,rect.GetCenter().y - rect.GetRadius().y,circle.GetCenter().x,circle.GetCenter().y) >= fl ) )
				{
					nResult = false;
				}
			}
			else
			{
				// 左下
				if ( ( circle.GetCenter().y > rect.GetCenter().y + rect.GetRadius().y ) )
				{
					if ( ( DistanceSqrf(rect.GetCenter().x - rect.GetRadius().x,rect.GetCenter().y + rect.GetRadius().y,circle.GetCenter().x,circle.GetCenter().y) >= fl ) )
					{
						nResult = false;
					}
				}
			}
		}
		else
		{
			// 右
			if ( circle.GetCenter().x > rect.GetCenter().x + rect.GetRadius().x )
			{
				// 右上
				if ( ( circle.GetCenter().y < rect.GetCenter().y - rect.GetRadius().y ) )
				{
					if ( ( DistanceSqrf(rect.GetCenter().x + rect.GetRadius().x,rect.GetCenter().y - rect.GetRadius().y,circle.GetCenter().x,circle.GetCenter().y) >= fl ) )
					{
						nResult = false;
					}
				}
				else
				{
					// 右下
					if ( ( circle.GetCenter().y > rect.GetCenter().y + rect.GetRadius().y ) )
					{
						if ( ( DistanceSqrf(rect.GetCenter().x + rect.GetRadius().x,rect.GetCenter().y + rect.GetRadius().y,circle.GetCenter().x,circle.GetCenter().y) >= fl ) )
						{
							nResult = false;
						}
					}
				}
			}
		}
	}

	return nResult;
}
