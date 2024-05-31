#include"PlayerBullet.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"

void PlayerBullet::Initialize(Vector2 velocity,Vector2 position,uint32_t life)
{
	velocity_ = velocity;

	position_ = position;

	life_ = life;

	chageTimer_ = 5;
}

void PlayerBullet::Update()
{
	Attack();
}

void PlayerBullet::Attack()
{

	if ( --life_ <= 0 )
	{
		isDead_ = true;
	}
	 position_+= velocity_;
}

void PlayerBullet::Draw()
{
	DrawBox(object.center_->x-10,position_.y-10,object.center_->x+10,position_.y+10,GetColor(255,0,0),true);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

