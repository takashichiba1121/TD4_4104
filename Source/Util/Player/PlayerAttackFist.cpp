#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"BaseEnemy.h"
void PlayerAttackFist::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(COLISION_SIZE_ / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);
}
void PlayerAttackFist::AttackInit(const Vector2& playerPos,bool direction,float pow)
{
	if (isAttack_==false)
	{
		isAttack_ = true;

		if ( direction )
		{
			DrawPos_ = { playerPos.x + ATTACK_POS_.x + COLISION_SIZE_.x / 2,playerPos.y + ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos.x - ATTACK_POS_.x - COLISION_SIZE_.x / 2,playerPos.y + ATTACK_POS_.y };
		}

		playerPow_ = pow;
	}
}

void PlayerAttackFist::Attack()
{
	if (isAttack_ )
	{
		AttackTime_++;

		if (AttackTime_> LAST_ATTACK_TIME_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackFist::Draw()
{
	if ( isAttack_ )
	{
			DrawBox(DrawPos_.x-COLISION_SIZE_.x/2,DrawPos_.y - COLISION_SIZE_.y/2,DrawPos_.x + COLISION_SIZE_.x/2,DrawPos_.y + COLISION_SIZE_.y/2,GetColor(0,255,0),false);
	}
}

void PlayerAttackFist::OnCollision()
{
	if ( GetCollisionInfo().object->GetCollisionAttribute() & COLLISION_ATTRIBUTE_PLAYRE )
	{
		dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(playerPow_*POW);
	}
}
