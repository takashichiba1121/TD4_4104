#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"

void FlyEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });


	CollisionManager::GetInstance()->AddObject(this);
	speed_.x = 3;
	velocity_ = { 1,0 };
	pos_ = { 100,100 };
	islive_ = true;
}

void FlyEnemy::Update()
{
	if ( isAttack_ )
	{
		Attack();
	}
	else
	{
		Move();
		if ( attackTime_ <= 0 )
		{
			isAttack_ = true;
		}
	}
}

void FlyEnemy::Move()
{
	if ( !islive_ ) return;
	if ( !playerPosPtr_ ) return;
	velocity_.Normalize();

	//if( )
	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		velocity_ *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		speed_.y = 0;
	}

	SetMapChipSpeed({ velocity_.x * speed_.x,velocity_.y * speed_.y });

}

void FlyEnemy::Attack()
{
}

void FlyEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

}
