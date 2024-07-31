#pragma once

#include "BaseEnemy.h"
#include "Util.h"
#include "CircleShape.h"

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
	std::unique_ptr<CircleShape> searchArea_;
	Counter moveTimer_;
	std::array<Vector2,5> moveCheckPoint_;

	int8_t targetCheckPoint_;
	Counter moveTime;
	ObjectUserData user_;
public:

	void Initialize() override;
	void Update() override;
	void Move() override;
	void Attack() override;
	void Draw() override;
	void OnCollision() override;
};
