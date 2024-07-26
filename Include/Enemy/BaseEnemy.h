#pragma once
#include "BaseObject.h"
#include "Util.h"
#include <array>

class MapChip;
class BaseEnemy:public BaseObject
{
protected:
	MapChip* mapchip_;
	ObjectUserData name_;
	uint32_t id_;
	bool immortal_ = false;
	BaseObject* playerPtr_;
	int32_t attackPower_;
	int32_t defense_;
	int32_t immortalTime_;
	int8_t statusEffects_;
	std::array<Counter,END> effectTimer;
	int8_t effectDamageInterval = 10;
	int8_t effectDamage = 1;
	int32_t curseStack = 0;
public:
	virtual void Move() {};
	virtual void Attack() {};
	virtual void EffectUpdate();
	virtual void Damage(int32_t damage) override;
	void SetPos(Vector2 pos);
	void SetVelocity(Vector2 velocity);
	void SetPlayerPtr(BaseObject* ptr);
	void SetMapChip(MapChip* mapptr);
	void SetEffect(Effects effect);
	void ReleaseEffect(Effects effect);
	bool IsEffect(Effects effect);
	bool IsImmortal();
	int32_t GetImmortalTime();
	int32_t GetCurseStack();
};

