#include "EnemyBullet.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include "MapChip.h"
#include "Collision.h"


void EnemyBullet::SetVelocity(Vector2 velocity)
{
	velocity_ = velocity;
}

void EnemyBullet::Initialize()
{
	speed_ = 3;
	drawSize_ = {32,32};
	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	damage_ = 85;

	tag.tag = "EnemyBullet";
	userData_ = &tag;

}

void EnemyBullet::Update()
{
	hp_ = 1;
	velocity_.Normalize();
	pos_ += velocity_ * speed_;
	CollisionEnable();
	if ( mapchip_->GetPosElement(pos_.x,pos_.y ) != NONE)
	{
		islive_ = false;
	}
	shape_->SetCenter({ pos_.x , pos_.y });
}

void EnemyBullet::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(damage_);
			islive_ = false;
		}
	}
}
void EnemyBullet::Draw()
{
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.y / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);
}