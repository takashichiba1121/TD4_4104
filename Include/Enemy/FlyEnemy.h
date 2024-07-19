#pragma once

#include "BaseEnemy.h"
class FlyEnemy :public BaseEnemy
{
private:
	int64_t attackTimer_;
	int64_t attackIntervalTime_;
	int64_t tackleTime_;
	Vector2 targetPos_;
	Vector2 attackBeforePos_;
	bool isAttack_;
	bool back_;
	const int64_t ATTACK_INTERVAL = 120;
	const int64_t ATTACK_TIME = 10;
	RectShape* shape_;

	ObjectUserData user;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Attack() override;
	void Draw() override;
	void OnCollision() override;
};
