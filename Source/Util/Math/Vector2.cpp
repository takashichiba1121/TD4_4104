#include "Vector2.h"
#include <math.h>
Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float _x,float _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2(const Vector2& start,const Vector2& end)
{
	x = end.x - start.x;
	y = end.y - start.y;
}

float Vector2::GetLenge() const
{
	return sqrtf(x * x + y * y);
}

void Vector2::Normalize()
{
	float vecLen = GetLenge();
	if ( !vecLen )
	{
		return;
	}
	x = x / vecLen;
	y = y / vecLen;
}

Vector2 GetVector2d(const Vector2& startVec,const Vector2& endVec)
{
	return Vector2(endVec,startVec);
}

float GetVector2dDot(const Vector2& vec1,const Vector2& vec2)
{
		return vec1.x * vec2.x + vec1.y * vec2.y;
}

float GetVector2dCross(const Vector2& vec1,const Vector2& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x = x + vec.x;
	y = y + vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x = x - vec.x;
	y = y - vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2& Vector2::operator*=(float speed)
{
	x *= speed;
	y *= speed;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

Vector2& Vector2::operator/=(float lenge)
{
	x /= lenge;
	y /= lenge;
	return *this;
	// TODO: return ステートメントをここに挿入します
}





Vector2 operator- (const Vector2& a,const Vector2& b)
{
	Vector2 temp(a);
	return temp -= b;
}

Vector2 operator*(const Vector2& a,float speed)
{
	Vector2 temp(a);
	return temp *= speed;
}

Vector2 operator*(float speed,const Vector2& a)
{
	Vector2 temp(a);
	return temp *= speed;
}

Vector2 operator/(const Vector2& a,float lenge)
{
	Vector2 temp(a);
	return temp /= lenge;
}

Vector2 operator +(const Vector2& a,const Vector2& b)
{
	Vector2 temp(a);
	return temp += b;
}

Vector2 operator /(Vector2 a,float lenge)
{
	Vector2 temp(a);
	return temp /= lenge;
}

Vector2 operator *(Vector2 a,float speed)
{
	Vector2 temp(a);
	return temp *= speed;
}
