#pragma once
#include "IObject.h"
#include<BaseObject.h>
#include<BossBullet.h>

class BossLongRangeAttack
{
private:

	int32_t CHARGE_TIME;
	int32_t FREEZE_TIME;

	int32_t freezeTime_;
	int32_t chargeTime_;
	bool isAttack_;
	bool isShot_;
	Vector2 bossPos_;
	int32_t dir_;
	std::unique_ptr<BossBullet>bullet_;

public:

	BossLongRangeAttack();

	void Attack();
	void Initialize();
	void Update();
	void BulletUpdate();
	void Draw();
	void DebugDraw();

	void SetBossPos(const Vector2& pos);
	void SetDir(int32_t dir);

	void SetChargeTime(int32_t time);
	void SetFreezeTime(int32_t time);

	void SetBulletTime(int32_t time);
	void SetBulletSize(const Vector2& size);
	void SetBulletPower(int32_t power);
	void SetBulletSpeed(float speed);

	bool IsAttack() const;
	bool IsShot()const;

};