#include "Vector2d.h"
#include <math.h>
Vector2d::Vector2d()
{
}

Vector2d::Vector2d(float _x,float _y)
{
	x = _x;
	y = _y;
}

Vector2d::Vector2d(const Vector2d& start,const Vector2d& end)
{
	x = end.x - start.x;
}

float Vector2d::GetLenge() const
{
	return sqrtf(x * x + y * y);
}

void Vector2d::Normalize()
{
	float vecLen = GetLenge();
	if ( !vecLen )
	{
		return;
	}
	x = x / vecLen;
	y = y / vecLen;
}

Vector2d Vector2d::operator=(const Vector2d& other)
{
	Vector2d ret;
	ret.x = other.x;
	ret.y = other.y;
	return ret;
}

Vector2d GetVector2d(const Vector2d& startVec,const Vector2d& endVec)
{
	return Vector2d(endVec,startVec);
}

float GetVector2dDot(const Vector2d& vec1,const Vector2d& vec2)
{
		return vec1.x * vec2.x + vec1.y * vec2.y;
}

float GetVector2dCross(const Vector2d& vec1,const Vector2d& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

Vector2d& Vector2d::operator+=(const Vector2d& vec)
{
	x = x + vec.x;
	y = y + vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2d& Vector2d::operator-=(const Vector2d& vec)
{
	x = x - vec.x;
	y = y - vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2d& Vector2d::operator*=(float speed)
{
	x *= speed;
	y *= speed;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2d& Vector2d::operator/=(float lenge)
{
	x /= lenge;
	y /= lenge;
	return *this;
	// TODO: return ステートメントをここに挿入します
}





Vector2d operator- (const Vector2d& a,const Vector2d& b)
{
	Vector2d temp(a);
	return temp -= b;
}

Vector2d operator*(const Vector2d& a,float speed)
{
	Vector2d temp(a);
	return temp *= speed;
}

Vector2d operator*(float speed,const Vector2d& a)
{
	Vector2d temp(a);
	return temp *= speed;
}

Vector2d operator/(const Vector2d& a,float lenge)
{
	Vector2d temp(a);
	return temp /= lenge;
}

Vector2d operator +(const Vector2d& a,const Vector2d& b)
{
	Vector2d temp(a);
	return temp += b;
}

Vector2d operator /(Vector2d a,float lenge)
{
	Vector2d temp(a);
	return temp /= lenge;
}

Vector2d operator *(Vector2d a,float speed)
{
	Vector2d temp(a);
	return temp *= speed;
}
