#pragma once
#include <memory>
#include <string>
#include <stdint.h>
#include "Vector2.h"
#include "IObject.h"
#include "RectShape.h"
#include "IShape.h"
#include "Util.h"

struct ObjectUserData
{
	std::string tag;

};

class BaseObject:public IObject
{
protected:

	int32_t hp_;
	int32_t textureId_;
	bool islive_;
	Vector2 pos_;
	Vector2 hitboxSize_;
	Vector2 drawSize_ = { 35,61 };
	float speed_;
	float originalSpeed_;
	Vector2 velocity_;
	ObjectUserData tag;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	//攻撃耐性等の特殊な処理を必要とする場合を考慮し仮想化
	virtual void Damage(int32_t damage);
	virtual void Damage(int32_t damage,Effects effect);
	bool IsLive();
	Vector2 GetHitBoxSize();
	Vector2 GetDrawSize();
	int32_t GetHp();
	Vector2 GetPos();
};

