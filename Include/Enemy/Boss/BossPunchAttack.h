#pragma once
#include "IObject.h"
#include "RectShape.h"
#include<BaseObject.h>

class BossPunchAttack :public IObject
{
private:

	int32_t TIME;

	Vector2 size_;
	Vector2 offset_;
	int32_t time_;
	Vector2 bossPos_;
	Vector2 bossSize_;
	RectShape* shape_;
	int32_t dir_;
	ObjectUserData tag;
	int32_t attackPower_;
	bool isAttack_;
	bool isPreparation_;
	int32_t rightHandImg_;

public:

	void Attack();
	void Initialize();
	void Update();
	void Draw();
	void DebugDraw();

	void SetBossPos(const Vector2& pos);
	void SetBossSize(const Vector2& size);
	void SetTime(int32_t time);
	void SetSize(const Vector2& size);
	void SetDir(int32_t dir);
	void SetPower(int32_t power);
	void SetOffset(const Vector2& offset);
	void Preparation();

	bool IsAttack() const;

	void OnCollision()override;

	~BossPunchAttack();

};

