#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
void PlayerAttackFist::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(COLISION_SIZE_ / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();
}
void PlayerAttackFist::AttackInit(const Vector2& playerPos,bool direction,float pow)
{
	if ( isAttack_ == false )
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

		CollisionEnable();
	}
}

void PlayerAttackFist::Attack()
{
	if ( isAttack_ )
	{
		AttackTime_++;

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > LAST_ATTACK_TIME_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
			isGiveDamage_ = false;
			CollisionDisable();
		}
	}

}

void PlayerAttackFist::Draw()
{
	if ( isAttack_ )
	{
		DrawBox(DrawPos_.x - COLISION_SIZE_.x / 2,DrawPos_.y - COLISION_SIZE_.y / 2,DrawPos_.x + COLISION_SIZE_.x / 2,DrawPos_.y + COLISION_SIZE_.y / 2,GetColor(0,255,0),false);
	}
}

void PlayerAttackFist::OnCollision()
{
	if ( GetCollisionInfo().userData && isGiveDamage_ == false )
	{
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "FlyEnemy" )
		{
			dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW);

			isGiveDamage_ = true;
		}
	}
}
