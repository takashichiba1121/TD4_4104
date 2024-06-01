#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"

void FlyEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });


	CollisionManager::GetInstance()->AddObject(this);
	speed.x = 3;
	velocity = { 1,0 };
	pos_ = { 100,100 };
	islive_ = true;
}

void FlyEnemy::Update()
{
	Move();
}

void FlyEnemy::Move()
{
	if ( !islive_ ) return;
	velocity.Normalize();


	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		velocity *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		speed.y = 0;
	}

	SetMapChipSpeed({ velocity.x * speed.x,velocity.y * speed.y });

}

void FlyEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

}
