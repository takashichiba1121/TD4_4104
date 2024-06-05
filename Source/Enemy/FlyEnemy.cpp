#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Player.h"

void FlyEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });


	CollisionManager::GetInstance()->AddObject(this);
	speed_ = 1;
	velocity_ = { 1,0 };
	islive_ = true;
	attackIntervalTime_ = ATTACK_INTERVAL;
	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });

	if ( GetRand(2) >= 2 )
	{
		velocity_ = { 1,0 };
	}
	else
	{
		velocity_ = { -1,0 };
	}
	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;
}

void FlyEnemy::Update()
{
	if ( isAttack_ )
	{
		Attack();
	}
	else
	{
		attackIntervalTime_--;
		if ( attackIntervalTime_ <= 0 && playerPtr_)
		{
			back_ = false;
			isAttack_ = true;
			targetPos_ = playerPtr_->GetPos();
			attackIntervalTime_ = ATTACK_INTERVAL;
			attackTimer_ = 0;
			attackBeforePos_ = pos_;
		}
	}

	Move();

}

void FlyEnemy::Move()
{
	if ( !islive_ ) return;
	velocity_.Normalize();
	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		velocity_.x *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM && !isAttack_)
	{
		velocity_.y = 0;
	}
	SetMapChipSpeed({ velocity_.x * speed_,velocity_.y * speed_ });
	

}

void FlyEnemy::Attack()
{
	if ( !back_ )
	{
		velocity_ = GetVector2d(targetPos_,pos_);
	}
	if ( ATTACK_TIME >= attackTimer_ )
	{
		attackTimer_++;
	}
	if ( GetOnDir() & 0b1 << OnDir::BOTTOM && !back_)
	{
		velocity_.y = -1;
		back_ = true;
		attackTimer_ = 0;
	}
	if ( velocity_.y > 0 )
	{
		speed_ = InQuad(0.f,6.f,ATTACK_TIME,attackTimer_);
	}
	else
	{
		speed_ = OutQuad(0.f,5.f,ATTACK_TIME,attackTimer_);
	}
	if ( pos_.y <= attackBeforePos_.y && velocity_.y < 0  )
	{
		isAttack_ = false;
		velocity_.y = 0.f;
		speed_ = GetRand(4) + 1;
	}


}

void FlyEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);
}

void FlyEnemy::OnCollision()
{
	if ( static_cast<ObjectUserData*>(GetCollisionInfo().userData)->tag == "player")
	{
		dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
	}
}