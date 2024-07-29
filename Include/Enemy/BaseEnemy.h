#pragma once
#include "BaseObject.h"
#include "Util.h"
#include <array>

enum ActionMode
{
	STAY,
	MOVE,
	APPROACH,
	ATTACK
};

class MapChip;
class Player;
class BaseEnemy:public BaseObject
{
protected:
	MapChip* mapchip_;
	ObjectUserData name_;
	uint32_t id_;
	bool immortal_ = false;
	Player* playerPtr_;
	int32_t attackPower_;
	int32_t defense_;
	int32_t immortalTime_;
	int8_t statusEffects_;
	std::array<Counter,END> effectTimer;
	const std::array<int32_t,END> effectTime = {360,240,600,360,0};
	const std::array<int32_t,END> effectDamageInterval = { 120,240,120,0,0 };
	std::array<float,END> effectDamage = { 0.15f,0.5f,0.25f,0.f,0.f };
	int32_t curseStack = 0;
	int32_t actionMode = MOVE;
public:
	virtual void Move() {};
	virtual void Attack() {};
	virtual void EffectUpdate();
	virtual void Damage(int32_t damage,Effects effects = END) override;
	void SetPos(Vector2 pos);
	void SetVelocity(Vector2 velocity);
	void SetPlayerPtr(Player* ptr);
	void SetMapChip(MapChip* mapptr);
	void SetEffect(Effects effect);
	void ReleaseEffect(Effects effect);
	bool IsEffect(Effects effect);
	bool IsImmortal();
	int32_t GetImmortalTime();
	int32_t GetCurseStack();
};

