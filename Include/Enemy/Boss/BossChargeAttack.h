#pragma once
#include "IObject.h"
#include "RectShape.h"
#include<BaseObject.h>

class BossChargeAttack :public IObject
{
private:
	float SPEED;
	int32_t TIME;

	bool isAttack_;
	bool isPreparation_;

	Vector2 size_;
	int32_t time_;

	Vector2 pos_;
	RectShape* shape_;
	int32_t dir_;
	ObjectUserData tag;
	int32_t attackPower_;
	Vector2 velocity_;
	float speed_;
	float posY_;

	int32_t chargeImg_;
	int32_t effectImg_;
	Vector2 drawSize_;

	int32_t anime_;
	int32_t animeTimer_;
	int32_t animeNum_ = 0;
	int32_t animeFrame_;

	int32_t anime2Timer_;
	int32_t anime2Num_ = 0;
	int32_t anime2Frame_;
	int32_t anime2_;

public:
	void Attack();
	void Initialize();
	void Update();
	void Draw();
	void DebugDraw();
	void Preparation();

	void SetBossPos(const Vector2& pos);
	void SetTime(int32_t time);
	void SetSize(const Vector2& size);
	void SetDir(int32_t dir);
	void SetPower(int32_t power);
	void SetSpeed(float speed);
	void SetAnimeFrame(int32_t frame);
	void SetAnime2Frame(int32_t frame);

	bool IsAttack()const;

	const Vector2& GetPos()const;

	void OnCollision()override;

	~BossChargeAttack();

private:

	void AnimeUpdate();

};