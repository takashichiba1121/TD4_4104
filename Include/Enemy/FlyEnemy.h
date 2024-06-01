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
	const int64_t ATTACK_INTERVAL = 120;
	const int64_t ATTACK_TIME = 15;
	RectShape* shape_;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Attack() override;
	void Draw() override;
};
