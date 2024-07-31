#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Player.h"
#include "Collision.h"

using namespace std;
void FlyEnemy::Initialize()
{
	hp_ = 1;

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
	searchArea_ = make_unique<CircleShape>();
	searchArea_->SetRadius({ ( drawSize_.x * 3 / 2 )});
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;

	moveTimer_.SetEndCount(20);
	beforeAttackFrame_ = 40;
	attackInterval_ = 120;
	attackFrame_ = 5;

	tag.tag = "Enemy";
	userData_ = &tag;

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
	else if(actionMode != ATTACK)
	{
		targetPos_ = Vector2(playerPtr_->GetPos(),pos_);
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

	velocity_ = Vector2(moveCheckPoint_[targetCheckPoint_ ],pos_);
	velocity_.Normalize();
	speed_ = InQuad(0,2,moveTimer_.GetCount(),moveTimer_.GetEndCount());

	pos_ += velocity_ * speed_;


	if ( Vector2(pos_,moveCheckPoint_[ targetCheckPoint_ ]).GetLenge() <= 10 )
	{
		int num = targetCheckPoint_;
		while ( targetCheckPoint_ == num )
		{
			targetCheckPoint_ = GetRand(moveCheckPoint_.size() - 1);
		}
		moveTimer_.ReSetCount();
	}

	shape_->SetCenter(pos_);

}

void FlyEnemy::Attack()
{
	if ( !beforeAttackCounter_.IsCountEnd() )
	{
		beforeAttackCounter_.CountUp();
	}
	else if ( !attackCounter_.IsCountEnd() )
	{
		attackCounter_.CountUp();
		speed_ = InQuad(0,5,attackCounter_.GetCount(),attackCounter_.GetEndCount());
		Vector2 targetVelo(targetPos_,pos_);
		targetVelo.Normalize();
		pos_ += targetVelo * speed_;
		attackIntervalCounter_.SetEndCount(attackInterval_);
	}
	else if(Vector2(pos_,targetPos_).GetLenge() <= 10 )
	{
		actionMode = MOVE;
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