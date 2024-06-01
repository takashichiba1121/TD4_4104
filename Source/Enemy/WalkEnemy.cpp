#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
void WalkEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });
	

	CollisionManager::GetInstance()->AddObject(this);
	gravity = { 0,1 };
	speed = 3;
	velocity = { 1,0 };
	pos_ = { 100,100 };
	islive_ = true;
}

void WalkEnemy::Update()
{
	Move();
}

void WalkEnemy::Move()
{
	if ( !islive_ ) return;
	velocity.Normalize();

	gravity.y += 0.5f;
	gravity.y = max(gravity.y,4);

	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT)
	{
		velocity *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity = { 0,0 };
	}

	SetMapChipSpeed({ velocity * speed,gravity });

}

void WalkEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

}
