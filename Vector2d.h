#pragma once

class Vector2d
{
public:
	float x = 0;
	float y = 0;

	Vector2d();
	Vector2d(float _x,float _y);
	Vector2d(const Vector2d& start,const Vector2d& end);



	float GetLenge() const;
	void VecNorm();

	Vector2d operator +();

	Vector2d operator -();

	Vector2d& operator +=(const Vector2d& other);

	Vector2d& operator -=(const Vector2d& other);

	Vector2d& operator *=(float speed);

	Vector2d& operator /=(float Lenge);

};

Vector2d GetVector2d(const Vector2d& startVec,const Vector2d& endVec);

float GetVector2dDot(const Vector2d& vec1,const Vector2d& vec2);

float GetVector2dCross(const Vector2d& vec1,const Vector2d& vec2);

Vector2d operator +(const Vector2d& a,const Vector2d& b);

Vector2d operator -(const Vector2d& a,const Vector2d& b);

Vector2d operator *(const Vector2d& a,const float& speed);

Vector2d operator *(const float& speed,const Vector2d& a);

Vector2d operator /(const Vector2d& a,float Lenge);