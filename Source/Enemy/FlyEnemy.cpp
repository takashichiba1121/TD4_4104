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
	pos_.x = GetRand(850) + 50.f;
	pos_.y = 100.f;
	for ( int i = 0; i < moveCheckPoint_.size(); i++ )
	{
		int8_t r = GetRand(200) + 300;
		float theta = 2 * PI * ( GetRand(120) / 120 + (i * 0.15f));


		moveCheckPoint_[ i ].x = r * cosf(theta) + pos_.x;
		moveCheckPoint_[ i ].y = r * sinf(theta) + pos_.y;
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

	if ( Collision::Circle2Circle(*playerPtr_->GetCircleShape() ,*searchArea_.get()))
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
	if (!moveTimer_.IsCountEnd() )
	{
		moveTimer_.CountUp();
	}

	velocity_ = Vector2(pos_,moveCheckPoint_[targetCheckPoint_ ]);
	velocity_.Normalize();
	speed_ = InQuad(0,2,moveTimer_.GetEndCount(),moveTimer_.GetCount());

	pos_ += velocity_ * speed_;


	if ( Vector2(pos_,moveCheckPoint_[ targetCheckPoint_ ]).GetLenge() <= 40 )
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

		speed_ = InQuad(0,5,attackCounter_.GetEndCount(),attackCounter_.GetCount());

		attackIntervalCounter_.SetEndCount(attackInterval_);
	}
	else if(Vector2(pos_,targetPos_).GetLenge() <= 10 )
	{
		actionMode = MOVE;
	}

	if ( beforeAttackCounter_.IsCountEnd() )
	{
		Vector2 targetVelo(targetPos_,pos_);
		targetVelo.Normalize();
		pos_ += targetVelo * speed_;
	}
}

void FlyEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,155),true);
	DrawFormatString(100,100,0xffffff,"%f",Vector2(pos_,moveCheckPoint_[ targetCheckPoint_ ]).GetLenge());
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