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
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	damage_ = 100;
}

void EnemyBullet::Update()
{
	pos_ += velocity_ * speed_;


}

void EnemyBullet::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(damage_);
		}
	}
}
void EnemyBullet::Draw()
{

}