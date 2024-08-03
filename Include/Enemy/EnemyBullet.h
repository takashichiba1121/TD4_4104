#pragma once
#include "BaseEnemy.h"
class EnemyBullet:public BaseEnemy
{
private:
	int32_t damage_;
	RectShape* shape_;
public:
	void SetVelocity(Vector2 velocity);
	void Initialize() override;
	void Update() override;
	void Draw(Vector2 scroll) override;
	void OnCollision() override;

};

