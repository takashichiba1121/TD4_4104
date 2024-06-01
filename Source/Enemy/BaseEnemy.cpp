#include "BaseEnemy.h"

void BaseEnemy::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void BaseEnemy::SetPlayerPtr(BaseObject* ptr)
{
	playerPtr_ = ptr;
}
