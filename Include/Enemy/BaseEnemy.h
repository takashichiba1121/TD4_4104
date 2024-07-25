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
	int32_t immotalTime_;
	int8_t statusEffects_;
	std::array<Counter,ICED + 1> effectTimer;

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
};

