#include "Vec2.h"

#include<algorithm>

#include<imgui.h>

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

const float2 operator+(const float2& v1,const float2& v2)
{
	float2 tmp(v1);
	return tmp += v2;
}

float2 Min(const float2& v1,const float2& v2)
{
	return float2(std::min(v1.x,v2.x),std::min(v1.y,v2.y));
}

float2 Min(const float2& v1,float f)
{
	return float2(std::min(v1.x,f),std::min(v1.y,f));
}

float2 Max(const float2& v1,const float2& v2)
{
	return float2(std::max(v1.x,v2.x),std::max(v1.y,v2.y));
}

float2 Max(const float2& v1,float f)
{
	return float2(std::max(v1.x,f),std::max(v1.y,f));
}

const int2 operator+(const int2& v1,const int2& v2)
{
	int2 tmp(v1);
	return tmp += v2;
}

const int2 operator-(const int2& v1,const int2& v2)
{
	int2 tmp(v1);
	return tmp -= v2;
}

float2& float2::operator+=(const float2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

float2::operator ImVec2()
{
	ImVec2 ret;

	ret.x = x;
	ret.y = y;

	return ret;
}
