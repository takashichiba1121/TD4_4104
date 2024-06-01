#include "BaseObject.h"
void BaseObject::Damage(int32_t damage)
{
	hp_ -= damage;
}
bool BaseObject::IsLive()
{
	if ( hp_ <= 0 )
	{
		islive_ = false;
	}
	return islive_;
}

Vector2 BaseObject::GetHitBoxSize()
{
	return hitboxSize_;
}

Vector2 BaseObject::GetDrawSize()
{
	return drawSize_;
}


int BaseObject::GetHp()
{
	return hp_;
}

Vector2 BaseObject::GetPos()
{
	return pos_;
}

IShape* BaseObject::GetShape() const
{
	return shape_;
}
