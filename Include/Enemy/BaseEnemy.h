#pragma once
#include "BaseObject.h"

class BaseEnemy:public BaseObject
{
protected:
	std::string tag_;
	uint32_t id_;
	int32_t hp_;
	int32_t speed_;
	Vector2 velocity_;
	int32_t textureId_;
	bool islive_;
	Vector2 pos_;
	Vector2 hitboxSize_;
	Vector2 drawSize_ = { 35,61 };
	IObject* playerPosPtr_;
	RectShape* shape_;
	int32_t attackPower_;
	int32_t defense_;
};

