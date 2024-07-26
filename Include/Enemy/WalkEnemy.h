#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
class WalkEnemy:public BaseEnemy
{
private:
	Vector2 gravity_;
	RectShape* shape_;

	int8_t nextElement;
	ObjectUserData user;

	bool tern = false;
	int8_t ternInverval;
	int8_t ternInvervalTimer;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Draw() override;
	void OnCollision() override;
};

