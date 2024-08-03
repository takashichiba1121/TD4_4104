#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Player.h"
#include "Collision.h"
#include "GameConfig.h"
#include "EnemyManager.h"

using namespace std;
void FlyEnemy::Initialize()
{
	animeNum_ = 4;
	animeSpeed_ = 5;
	drawSize_ = { 64,64 };

	maxHp_ = 1;
	hp_ = 1;
	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	pos_.x = GameConfig::GetWindowWidth() / 2;
	pos_.y = GameConfig::GetWindowHeight() / 2+ 50;
	for ( size_t i = 0; i < moveCheckPoint_.size(); i++ )
	{
		int8_t r = GetRand(200) + 500;
		float theta = 2 * PI * ( GetRand(60) / 360 + (i * 0.15f));


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
	attackPower_ = 100;

	moveTimer_.SetEndCount(20);
	beforeAttackFrame_ = 40;
	attackInterval_ = 120;
	attackFrame_ = 5;

	tag.tag = "Enemy";
	userData_ = &tag;

}

void FlyEnemy::Update()
{
	if ( !islive_ || !playerPtr_ ) return;
	immortalTime_--;
	attackIntervalCounter_.CountUp();
	isCursedDamage_ = false;

	searchArea_->SetCenter({ pos_.x,pos_.y });

	if ( Collision::Circle2Circle(*playerPtr_->GetCircleShape() ,*searchArea_.get()) && attackIntervalCounter_.IsCountEnd())
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
		targetPos_ = playerPtr_->GetPos();
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

	AnimeUpdate();
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
	attackIntervalCounter_.SetEndCount(attackInterval_);
	if ( !beforeAttackCounter_.IsCountEnd() )
	{
		attackFinish_ = false;
		beforeAttackCounter_.CountUp();
		speed_ = 0;
	}
	else if ( !attackCounter_.IsCountEnd() )
	{
		attackCounter_.CountUp();

		speed_ = InQuad(0,5,attackCounter_.GetEndCount(),attackCounter_.GetCount());

		attackIntervalCounter_.SetEndCount(attackInterval_);
	}
	if ( !attackFinish_ )
	{
		speed_ = InQuad(0,5,attackCounter_.GetEndCount(),attackCounter_.GetCount());
		Vector2 targetVelo(pos_,targetPos_);
		targetVelo.Normalize();
		pos_ += targetVelo * speed_;
	}
	else
	{
		stanTimer_.CountUp();
		if ( stanTimer_.IsCountEnd() )
		{
			actionMode = MOVE;
		}
	}
	if ( Vector2(pos_,targetPos_).GetLenge() <= 25 )
	{
		attackFinish_ = true;
		stanTimer_.SetEndCount(30);
	}
}

void FlyEnemy::Draw(Vector2 scroll)
{
	if ( !islive_ || !playerPtr_) return;
	bool flag = false;
	if ( velocity_.x < 0 )
	{
		flag = true;
	}
	DrawRectRotaGraph(pos_.x - scroll.x,pos_.y - scroll.x,drawSize_.x * anime_,0,drawSize_.x,drawSize_.y,1,0,EnemyManager::GetTexHandle(FLY),true,flag);

	if ( playerPtr_->GetEyeTag() == PlayerEyeTags::Clairvoyance )
	{
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		pos_.x + drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),false);
		int32_t r = pos_.x + drawSize_.x / 2;
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		 r * (hp_/maxHp_),pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),true);
	}
}

void FlyEnemy::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast<UserData*>(GetCollisionInfo().userData)->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
			actionMode = MOVE;
		}
	}
}