#include "FlyEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"

void FlyEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });


	CollisionManager::GetInstance()->AddObject(this);
	speed_ = 3;
	velocity_ = { 1,0 };
	pos_ = { 100,100 };
	islive_ = true;
	attackTime_ = ATTACK_INTERVAL;
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
		attackTime_--;
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
	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		velocity_ *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		velocity_.y = 0;
	}

	SetMapChipSpeed({ velocity_.x * speed_,velocity_.y * speed_ });

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
