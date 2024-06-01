#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
void WalkEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });
	

	CollisionManager::GetInstance()->AddObject(this);
	gravity_ = { 0,1 };
	speed_ = 3;
	velocity_ = { 1,0 };
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
