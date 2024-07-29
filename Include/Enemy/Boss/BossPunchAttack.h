#pragma once
#include "IObject.h"
#include "RectShape.h"
#include<BaseObject.h>

class BossPunchAttack :public IObject
{
private:
	Vector2 size_;
	int32_t time_;
	bool isAttack_;
	Vector2 bossPos_;
	Vector2 bossSize_;
	RectShape* shape_;
	int32_t dir_;
	ObjectUserData tag;
	int32_t attackPower_;
public:

	void Attack();
	void Initialize();
	void Update();
	void Draw();

	void SetBossPos(const Vector2& pos);
	void SetBossSize(const Vector2& size);
	void SetTime(int32_t time);
	void SetSize(const Vector2& size);
	void SetDir(int32_t dir);
	void SetPower(int32_t power);

	void OnCollision()override;

};

