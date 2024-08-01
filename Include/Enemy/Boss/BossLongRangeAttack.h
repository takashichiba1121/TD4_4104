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

	void Attack();
	void Initialize();
	void Update();
	void Draw();

	void SetBossPos(const Vector2& pos);
	void SetTime(int32_t time);
	void SetDir(int32_t dir);

	bool IsAttack() const;

};