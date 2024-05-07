#include "Vec2.h"

int2& int2::operator+=(const int2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

int2& int2::operator-=(const int2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

int2& int2::operator/=(const int2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

bool int2::operator==(const int2& v)
{
	return v.x == x && v.y == y;
}

const int2 operator+(const int2& v1, const int2& v2)
{
	int2 tmp(v1);
	return tmp += v2;
}

const int2 operator-(const int2& v1, const int2& v2)
{
	int2 tmp(v1);
	return tmp -= v2;
}
