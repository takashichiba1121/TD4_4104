#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
#include"WalkEnemy.h"
void PlayerAttackFist::Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;

	velocity_ = velocity;

	shape_ = new RectShape();

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();
}
void PlayerAttackFist::AttackInit(float pow)
{
	if (INTERVAL_<=AttackInterval_ )
	{
		playerPow_ = pow;

		isAttack_ = true;

		AttackTime_ = 0;

		if ( *direction_ )
		{
			velocity_->x += MOVE_;
		}
		else
		{
			velocity_->x -= MOVE_;
		}

		CollisionEnable();
	}
}

void PlayerAttackFist::Attack()
{

	if (isAttack_ )
	{
		if ( *direction_ )
		{
			DrawPos_ = { playerPos_->x + ATTACK_POS_.x + COLISION_SIZE_.x / 2,playerPos_->y + ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos_->x - ATTACK_POS_.x - COLISION_SIZE_.x / 2,playerPos_->y + ATTACK_POS_.y };
		}

		AttackTime_++;

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > LAST_ATTACK_TIME_ )
		{
			CollisionDisable();
			isAttack_ = false;
		}
	}
	else
	{
		AttackInterval_++;
	}
}

void PlayerAttackFist::Draw()
{
	if ( isAttack_ )
	{
		DrawBox(DrawPos_.x - COLISION_SIZE_.x / 2,DrawPos_.y - COLISION_SIZE_.y / 2,
			DrawPos_.x + COLISION_SIZE_.x / 2,DrawPos_.y + COLISION_SIZE_.y / 2,
			GetColor(0,255,0),false);
	}
}

void PlayerAttackFist::OnCollision()
{
	if ( GetCollisionInfo().userData && isGiveDamage_ == false )
	{

		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "FlyEnemy" )
		{
			dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);
			isGiveDamage_ = true;
		}
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "WalkEnemy" )
		{
			dynamic_cast< WalkEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);

			isGiveDamage_ = true;
		}
	}
}