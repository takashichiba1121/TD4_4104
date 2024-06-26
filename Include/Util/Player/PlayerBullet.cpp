#include"PlayerBullet.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"
#include"CollisionConfig.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
#include"WalkEnemy.h"

void PlayerBullet::Initialize(Vector2 velocity,Vector2 position,uint32_t life,float playerPow)
{
	velocity_ = velocity;

	position_ = position;

	playerPow_ = playerPow;

	life_ = life;

	chageTimer_ = 5;

	//MapChipObjectEnable();
	//SetMapChipCenter(&position_);
	//SetMapChipRadius({ SIZE_.x / 2,SIZE_.y / 2 });

	shape_ = new RectShape();
	shape_->SetRadius(SIZE_ / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);
}

void PlayerBullet::Update()
{
	Attack();

	shape_->SetCenter(position_);
}

void PlayerBullet::Attack()
{

	if ( --life_ <= 0 )
	{
		isDead_ = true;
	}
	position_ += velocity_;
}

void PlayerBullet::Draw()
{
	DrawBox(position_.x - SIZE_.x,position_.y - SIZE_.y,position_.x + SIZE_.x,position_.y + SIZE_.y,GetColor(255,0,0),true);
}

void PlayerBullet::OnCollision()
{
	if (isDead_==false )
	{
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "FlyEnemy" )
		{
			dynamic_cast< FlyEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);
		}
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "WalkEnemy" )
		{
			dynamic_cast< WalkEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);
		}
	}
	isDead_ = true;
}

