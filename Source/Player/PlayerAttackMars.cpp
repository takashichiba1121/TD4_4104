#include "PlayerAttackMars.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
#include"WalkEnemy.h"
void PlayerAttackMars::Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)
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
void PlayerAttackMars::AttackInit(float pow,float changeCrit,float changeCdmg)
{
	if (INTERVAL_<=AttackInterval_ )
	{
		playerPow_ = pow;

		playerCrit_ = changeCrit;

		playerCdmg_ = changeCdmg;

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

void PlayerAttackMars::Attack()
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

void PlayerAttackMars::Draw(Vector2 scroll)
{
	if ( isAttack_ )
	{
		DrawBox(scroll.x + DrawPos_.x - COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y - COLISION_SIZE_.y / 2,
			scroll.x + DrawPos_.x + COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y + COLISION_SIZE_.y / 2,
			GetColor(0,255,0),false);
	}
}

void PlayerAttackMars::OnCollision()
{
}