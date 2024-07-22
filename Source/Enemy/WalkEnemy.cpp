#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include "MapChip.h"

using namespace std;
void WalkEnemy::Initialize()
{

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });

	ternInverval = 2;
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
	speed_ = 1;
	user.tag = "WalkEnemy";
	userData_ = &user;

	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;

	hp_ = 10;

}

void WalkEnemy::Update()
{
	immotalTime_--;
	if ( immotalTime_ <= 0 )
	{
		immortal_ = false;
	}
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
	 nextElement = mapchip_->GetPosElement(pos_.x +(( velocity_.x * speed_ )) + ( drawSize_.x / 2 ),
		pos_.y + ( drawSize_.y / 2 ) + 1);

	if ((nextElement == NEXT || nextElement == NONE) && !tern)
	{
		velocity_ *= -1;
		tern = true;
	}

	if ( tern )
	{
		ternInvervalTimer++;
		if ( ternInverval < ternInvervalTimer )
		{
			tern = false;
			ternInvervalTimer = 0;
		}
	}

	SetMapChipSpeed({ velocity_ * speed_,gravity_ });
	shape_->SetCenter(pos_);


	
}

void WalkEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);
	DrawFormatString(100,100,0xffffff,"%d",nextElement);


}

void WalkEnemy::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}
