#pragma once
#include "BaseObject.h"
#include "Util.h"

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
public:
	virtual void Move() {};
	virtual void Attack() {};
	virtual void Damage(int32_t damage) override;
	void SetPos(Vector2 pos);
	void SetVelocity(Vector2 velocity);
	void SetPlayerPtr(BaseObject* ptr);
	void SetMapChip(MapChip* mapptr);
	void SetEffect(Effects effect);
	void OutEffect(Effects effect);
};

