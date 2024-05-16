#pragma once


struct ImVec2;

class float2
{
public:

	float x;
	float y;

	float2& operator+=(const float2& v);

	 operator ImVec2();
};

const float2 operator+(const float2& v1,const float2& v2);


float2 Min(const float2& v1,const float2& v2);
float2 Min(const float2& v1,float f);

float2 Max(const float2& v1,const float2& v2);
float2 Max(const float2& v1,float f);

class int2
{
public:

	union
	{
		struct
		{
			int x;
			int y;
		};

		int data[ 2 ];
	};

	int2& operator+=(const int2& v);
	int2& operator-=(const int2& v);
	int2& operator/=(const int2& v);

	bool operator==(const int2& v);

};

const int2 operator+(const int2& v1,const int2& v2);
const int2 operator-(const int2& v1,const int2& v2);

