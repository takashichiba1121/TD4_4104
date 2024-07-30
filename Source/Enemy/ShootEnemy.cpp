#include "ShootEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include "MapChip.h"
#include "Collision.h"

using namespace std;
void ShootEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });

	ternInverval_ = 2;
	gravity_ = { 0,1 };
	originalSpeed_ = GetRand(4) + 1;
	if ( GetRand(2) >= 2 )
	{
		velocity_ = { 1,0 };
	}
	else
	{
		velocity_ = { -1,0 };
	}
	user_.tag = "WalkEnemy";
	userData_ = &user_;

	islive_ = true;

	searchArea_ = make_unique<RectShape>();
	searchArea_->SetRadius({ ( drawSize_.x * 5 / 2 ),drawSize_.y / 2 });
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 100;

	attackInterval_ = 45;
	beforeAttackFrame_ = 5;
	attackFrame_ = 25;

	hp_ = 70;

}

void ShootEnemy::Update()
{
	if ( !islive_ ) return;
	immortalTime_--;
	attackIntervalCounter_.CountUp();


	searchArea_->SetCenter({ ( sign(-velocity_.x) * searchArea_->GetRadius().x ) + pos_.x,pos_.y });

	attackArea_->SetCenter({ ( sign(-velocity_.x) * attackArea_->GetRadius().x ) + pos_.x,pos_.y });

	if ( Collision::Rect2Rect(*dynamic_cast< RectShape* >( playerPtr_->GetShape() ),*attackArea_.get()) && attackIntervalCounter_.IsCountEnd() )
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

	for ( auto& itr : bullets )
	{
		itr->Update();
	}

	EffectUpdate();
}

void ShootEnemy::Move()
{

	velocity_.Normalize();

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT)
	{
		velocity_ *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
		prevElement_ = mapchip_->GetPosElement(pos_.x,pos_.y + ( drawSize_.y / 2 ) + 1);
	}
	 nextElement_ = mapchip_->GetPosElement(pos_.x +(( velocity_.x * speed_ )) + ( drawSize_.x / 2 ),
		pos_.y + ( drawSize_.y / 2 ) + 1);

	if ((nextElement_ == NEXT || (nextElement_ == NONE && GetOnDir() & 0b1 << OnDir::BOTTOM )) && !tern_ )
	{
		velocity_ *= -1;
		tern_ = true;
	}

	if ( prevElement_ != NONE )
	{
		velocity_.y = 0;
		gravity_.y = 0;
	}


	if ( tern_ )
	{
		ternInvervalTimer_++;
		if ( ternInverval_ < ternInvervalTimer_ )
		{
			tern_ = false;
			ternInvervalTimer_ = 0;
		}
	}

	SetMapChipSpeed({ velocity_ * speed_,gravity_ });
	shape_->SetCenter(pos_);

	
}


void ShootEnemy::Attack()
{
	
	if ( !beforeAttackCounter_.IsCountEnd() )
	{
		beforeAttackCounter_.CountUp();
	}
	else if ( !attackCounter_.IsCountEnd() )
	{
		attackCounter_.CountUp();
		unique_ptr<EnemyBullet> bullet = make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetMapChip(mapchip_);
		bullet->SetVelocity(Vector2(pos_,playerPtr_->GetPos()));
		bullets.push_back(move(bullet));
		attackIntervalCounter_.SetEndCount(attackInterval_);
	}
	else
	{
		actionMode = MOVE;
	}
	SetMapChipSpeed({ 0.f,gravity_.y });

}

void ShootEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.y / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

	for ( auto& itr : bullets )
	{
		itr->Draw();
	}

	DrawFormatString(100,100,0xffffff,"%d",nextElement_);


}

void ShootEnemy::OnCollision()
{
	//if ( GetCollisionInfo().userData )
	//{
	//	if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
	//	{
	//		dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
	//	}
	//}
}


