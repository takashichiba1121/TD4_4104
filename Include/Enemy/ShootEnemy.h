#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
class ShootEnemy:public BaseEnemy
{
private:
	Vector2 gravity_;
	RectShape* shape_;

	int8_t nextElement_;
	int8_t prevElement_;

	ObjectUserData user_;

	Counter attackCounter_;
	Counter beforeAttackCounter_;
	Counter attackIntervalCounter_;
	int32_t attackInterval_ = 60;
	int32_t beforeAttackFrame_ = 5;
	int32_t attackFrame_ = 25;

	bool tern_ = false;
	int8_t ternInverval_;
	int8_t ternInvervalTimer_;
	std::unique_ptr<RectShape> searchArea_;
	std::unique_ptr<RectShape> attackArea_;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Attack() override;
	void Draw() override;
	void OnCollision() override;
	void Approach();
};

