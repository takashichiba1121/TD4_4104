#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"

using namespace std;
void WalkEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });


	gravity_ = { 0,1 };
	speed_ = GetRand(4)+1;
	if ( GetRand(2) >= 2 )
	{
		velocity_ = { 1,0 };
	}
	else
	{
		velocity_ = { -1,0 };
	}
	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;

}

void WalkEnemy::Update()
{
	Move();
	shape_->SetCenter({ pos_.x , pos_.y });
}

void WalkEnemy::Move()
{
	if ( !islive_ ) return;
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
	}

	SetMapChipSpeed({ velocity_ * speed_,gravity_ });

}

void WalkEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

}

void WalkEnemy::OnCollision()
{
	if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "player" )
	{
		dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
	}
}
