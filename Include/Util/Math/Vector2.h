#pragma once

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float _x,float _y);
	Vector2(const Vector2& start,const Vector2& end);

	float GetLenge() const;
	void Normalize();


	Vector2& operator +=(const Vector2& other);

	Vector2& operator -=(const Vector2& other);

	Vector2& operator *=(float speed);

	Vector2& operator /=(float lenge);

};

Vector2 GetVector2d(const Vector2& startVec,const Vector2& endVec);

float GetVector2dDot(const Vector2& vec1,const Vector2& vec2);

float GetVector2dCross(const Vector2& vec1,const Vector2& vec2);

Vector2 operator +(const Vector2& a,const Vector2& b);

Vector2 operator -(const Vector2& a,const Vector2& b);

Vector2 operator *(const Vector2& a, float speed);

Vector2 operator *(float speed,const Vector2& a);

Vector2 operator /(const Vector2& a,float lenge);