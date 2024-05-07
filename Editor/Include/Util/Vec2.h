#pragma once
class float2
{
public:

	float x;
	float y;

};

class int2
{
public:

	int x;
	int y;

	int2& operator+=(const int2& v);
	int2& operator-=(const int2& v);
	int2& operator/=(const int2& v);

	bool operator==(const int2& v);

};

const int2 operator+(const int2& v1,const int2& v2);
const int2 operator-(const int2& v1,const int2& v2);
