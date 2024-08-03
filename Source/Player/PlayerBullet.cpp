#include"PlayerBullet.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"
#include"CollisionConfig.h"
#include"CollisionManager.h"
#include"BaseEnemy.h"

void PlayerBullet::Initialize(Vector2 velocity,Vector2 position,uint32_t life,float pow,float changeCrit,float changeCdmg,PlayerBullet::Type type)
{
	velocity_ = velocity;

	position_ = position;

	pow_ = pow;

	playerCrit_ = changeCrit;

	playerCdmg_ = changeCdmg;

	life_ = life;

	type_ = type;

	shape_ = new RectShape();
	shape_->SetRadius(SIZE_ / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);
}

void PlayerBullet::Update()
{
	if ( --life_ <= 0 )
	{
		isDead_ = true;
	}
	position_ += velocity_;

	shape_->SetCenter(position_);
}

void PlayerBullet::Draw(Vector2 scroll)
{
	DrawBox(scroll.x+position_.x - SIZE_.x,scroll.y+position_.y - SIZE_.y,scroll.x+position_.x + SIZE_.x,scroll.y+position_.y + SIZE_.y,GetColor(255,0,0),true);
}

void PlayerBullet::OnCollision()
{

	if ( isDead_ == false )
	{
		switch ( type_ )
		{
		case PlayerBullet::Type::Normal:
			if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "Enemy" )
			{
				if ( GetRand(1000) <= playerCrit_ * 1000 )
				{
					dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(pow_ + ( pow_ * playerCdmg_ ));
				}
				else
				{
					dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(pow_);
				}
			}
			break;
		case PlayerBullet::Type::ICED:
			if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "Enemy" )
			{
				dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(pow_,Effects::ICED);
			}
			break;
		case PlayerBullet::Type::BURN:
			if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "Enemy" )
			{
				dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(pow_,Effects::BURN);
			}
			break;
		default:
			break;
		}
		isDead_ = true;
	}
}

