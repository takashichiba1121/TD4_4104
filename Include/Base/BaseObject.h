#pragma once
#include <memory>
#include <string>
#include <stdint.h>
#include "Vector2.h"
class BaseObject
{
protected:

	int32_t hp_;
	int32_t textureId_;
	bool islive_;
	Vector2 pos_;
	Vector2 hitboxSize_;
	Vector2 drawSize_ = { 35,61 };
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	//攻撃耐性等の特殊な処理を必要とする場合を考慮し仮想化
	virtual void Damage(int32_t damage);
	bool IsLive();
	Vector2 GetHitBoxSize();
	Vector2 GetDrawSize();
	int32_t GetHp();
	Vector2 GetPos();

};

