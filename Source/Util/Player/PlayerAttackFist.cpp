#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
#include"WalkEnemy.h"
void PlayerAttackFist::Initialize(Vector2* playerPos,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;

	shape_ = new RectShape();

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();
}
void PlayerAttackFist::AttackInit(float pow)
{
	switch ( attackType_ )
	{
	case 0:
		attackType_ = 1;

		nextAttack_ = 0;

		playerPow_ = pow;

		isAttack_ = true;

		CollisionEnable();
		break;
	case 1:
		nextAttack_ = 2;
		break;
	case 2:
		nextAttack_ = 3;
		break;
	default:
		break;
	}

	//if ( isAttack_ == false )
	//{
	//	isAttack_ = true;

	//	if ( direction )
	//	{
	//		DrawPos_ = { playerPos_->x + combo1.ATTACK_POS_.x + combo1.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
	//	}
	//	else
	//	{
	//		DrawPos_ = { playerPos_->x - combo1.ATTACK_POS_.x - combo1.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
	//	}

	//	playerPow_ = pow;

	//	CollisionEnable();
	//}
}

void PlayerAttackFist::Attack()
{

	switch ( attackType_ )
	{
	case 1:
		if ( *direction_ )
		{
			DrawPos_ = { playerPos_->x + combo1.ATTACK_POS_.x + combo1.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos_->x - combo1.ATTACK_POS_.x - combo1.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
		}

		AttackTime_++;

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > combo1.LAST_ATTACK_TIME_ )
		{
			CollisionDisable();
			isAttack_ = false;
			if ( nextAttack_ == 2 )
			{
				AttackTime_ = combo1.INTERVAL_;
			}
		}
		if( AttackTime_ >= combo1.INTERVAL_)
		{
			if ( nextAttack_ == 0 )
			{
				attackType_ = 0;
				AttackTime_ = 0;
				isGiveDamage_ = false;
			}
			else if(nextAttack_==2)
			{
				attackType_ = nextAttack_;
				nextAttack_ = 0;
				AttackTime_ = 0;
				isGiveDamage_ = false;
				isAttack_ = true;
				CollisionEnable();
				shape_->SetRadius(combo2.COLISION_SIZE_/2);
			}
		}
		break;
	case 2:
		if ( *direction_ )
		{
			DrawPos_ = { playerPos_->x + combo2.ATTACK_POS_.x + combo2.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos_->x - combo2.ATTACK_POS_.x - combo2.COLISION_SIZE_.x / 2,playerPos_->y + combo1.ATTACK_POS_.y };
		}

		AttackTime_++;

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > combo2.LAST_ATTACK_TIME_ )
		{
			CollisionDisable();
			isAttack_ = false;
			if ( nextAttack_ == 3 )
			{
				AttackTime_ = combo2.INTERVAL_;
			}
		}
		if( AttackTime_ >= combo2.INTERVAL_)
		{
			if ( nextAttack_ == 0 )
			{
				attackType_ = 0;
				AttackTime_ = 0;
				isGiveDamage_ = false;
				CollisionEnable();
			}
			else if ( nextAttack_ == 3 )
			{
				attackType_ = nextAttack_;
				nextAttack_ = 0;
				AttackTime_ = 0;
				isGiveDamage_ = false;
				isAttack_ = true;
				shape_->SetRadius(combo3.COLISION_SIZE_ / 2);
			}
		}
		break;
	case 3:
		if ( *direction_ )
		{
			DrawPos_ = { playerPos_->x + combo3.ATTACK_POS_.x + combo3.COLISION_SIZE_.x / 2,playerPos_->y + combo3.ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos_->x - combo3.ATTACK_POS_.x - combo3.COLISION_SIZE_.x / 2,playerPos_->y + combo3.ATTACK_POS_.y };
		}

		AttackTime_++;

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > combo3.LAST_ATTACK_TIME_ )
		{
			attackType_ = 0;
			AttackTime_ = 0;
			isGiveDamage_ = false;
			isAttack_ = false;
			CollisionDisable();
		}
		break;
	default:
		break;
	}

}

void PlayerAttackFist::Draw()
{
	if (isAttack_ )
	{
		if ( attackType_ == 1 )
		{
			DrawBox(DrawPos_.x - combo1.COLISION_SIZE_.x / 2,DrawPos_.y - combo1.COLISION_SIZE_.y / 2,
				DrawPos_.x + combo1.COLISION_SIZE_.x / 2,DrawPos_.y + combo1.COLISION_SIZE_.y / 2,
				GetColor(0,255,0),false);
		}
		else if ( attackType_ == 2 )
		{
			DrawBox(DrawPos_.x - combo2.COLISION_SIZE_.x / 2,DrawPos_.y - combo2.COLISION_SIZE_.y / 2,
				DrawPos_.x + combo2.COLISION_SIZE_.x / 2,DrawPos_.y + combo2.COLISION_SIZE_.y / 2,
				GetColor(0,255,0),false);
		}
		else if ( attackType_ == 3 )
		{
			DrawBox(DrawPos_.x - combo3.COLISION_SIZE_.x / 2,DrawPos_.y - combo3.COLISION_SIZE_.y / 2,
				DrawPos_.x + combo3.COLISION_SIZE_.x / 2,DrawPos_.y + combo3.COLISION_SIZE_.y / 2,
				GetColor(0,255,0),false);
		}
	}
}

uint32_t PlayerAttackFist::GetInterval()
{
	switch ( attackType_ )
	{
	case 1:
		return combo1.INTERVAL_;
		break;
	case 2:
		return combo2.INTERVAL_;
		break;
	case 3:
		return combo3.INTERVAL_;
		break;
	default:
		break;
	}
	return 0;
}

float PlayerAttackFist::GetPow()
{
	switch ( attackType_ )
	{
	case 1:
		return combo1.POW;
		break;
	case 2:
		return combo2.POW;
	case 3:
		return combo3.POW;
		break;
	default:
		break;
	}
	return 0.0;
}

void PlayerAttackFist::OnCollision()
{
	if ( GetCollisionInfo().userData && isGiveDamage_ == false )
	{
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "FlyEnemy" )
		{
			switch ( attackType_ )
			{
			case 1:
				dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo1.POW);
				break;
			case 2:
				dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo2.POW);
				break;
			case 3:
				dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo3.POW);
				break;
			default:
				break;
			}
			isGiveDamage_ = true;
		}
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "WalkEnemy" )
		{
			switch ( attackType_ )
			{
			case 1:
				dynamic_cast< WalkEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo1.POW);
				break;
			case 2:
				dynamic_cast< WalkEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo2.POW);
				break;
			case 3:
				dynamic_cast< WalkEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * combo3.POW);
				break;
			default:
				break;
			}

			isGiveDamage_ = true;
		}
	}
}


