#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Player.h"
#include "Collision.h"


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
	hp_ = 10;
	if ( GetRand(2) >= 2 )
	{
		velocity_ = { 1,0 };
	}
	else
	{
		velocity_ = { -1,0 };
	}
	user_.tag = "FlyEnemy";
	userData_ = &user_;

	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);

	for ( int i = 0; i < moveCheckPoint_.size(); i++ )
	{
		int8_t r = GetRand(100) + 10;
		float theta = 2 * PI * ( GetRand(360) / 360 );

		moveCheckPoint_[ i ].x = r * cos(theta);
		moveCheckPoint_[ i ].y = r * sin(theta);
	}
	pos_ = moveCheckPoint_[ 0 ];
	targetCheckPoint_ = 0;

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;



}

void FlyEnemy::Update()
{
	if ( !islive_ ) return;
	immortalTime_--;
	attackIntervalCounter_.CountUp();


	searchArea_->SetCenter({ pos_.x,pos_.y });

	if ( Collision::Circle2Circle(*dynamic_cast< CircleShape* >( playerPtr_->GetShape() ),*searchArea_.get()))
	{
		if ( actionMode != ATTACK )
		{
			beforeAttackCounter_.SetEndCount(beforeAttackFrame_);
			attackCounter_.SetEndCount(attackFrame_);
		}
		actionMode = ATTACK;
	}

	if ( immortalTime_ <= 0 )
	{
		immortal_ = false;
	}

	if ( IsEffect(DELAY) )
	{
		speed_ = 1;
	}
	else
	{
		speed_ = originalSpeed_;
	}

	if ( !IsEffect(BIND) && !IsEffect(ICED) )
	{
		switch ( actionMode )
		{
		case MOVE:
			Move();
			break;
		case ATTACK:
			Attack();
			break;
		default:
			break;
		}

	}
	shape_->SetCenter({ pos_.x , pos_.y });


	EffectUpdate();
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

	velocity_ = Vector2();

	SetMapChipSpeed({ velocity_.x * speed_,velocity_.y * speed_ });
	shape_->SetCenter(pos_);

}

void FlyEnemy::Attack()
{
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
	if ( pos_.y - 1 <= attackBeforePos_.y && back_  )
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
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,155),true);
}

void FlyEnemy::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast<UserData*>(GetCollisionInfo().userData)->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}