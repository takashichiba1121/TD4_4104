#pragma once
#include "BaseEnemy.h"
#include "RectShape.h"
class WalkEnemy:public BaseEnemy
{
private:
	Vector2 gravity_;

	Vector2 nextPos_;

	RectShape* shape_;

	int8_t nextElement_;
	int8_t prevElement_;

	ObjectUserData user_;

	


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

