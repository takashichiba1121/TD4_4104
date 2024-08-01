#pragma once
#include "IObject.h"
#include "RectShape.h"
#include<BaseObject.h>

class BossBullet :public IObject
{
private:

	float SPEED;
	int32_t TIME;

	Vector2 size_;
	int32_t time_;
	bool isAttack_;
	Vector2 pos_;
	RectShape* shape_;
	int32_t dir_;
	ObjectUserData tag;
	int32_t attackPower_;
	Vector2 velocity_;
	float speed_;

public:
	void Attack();
	void Initialize();
	void Update();
	void Draw();

	bool IsAttack()const;

	void SetBossPos(const Vector2& pos);
	void SetDir(int32_t dir);

	void SetTime(int32_t time);
	void SetSize(const Vector2& size);
	void SetPower(int32_t power);
	void SetSpeed(float speed);

	void OnCollision()override;

};