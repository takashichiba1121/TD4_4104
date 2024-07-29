#include "BossPunchAttack.h"
#include<CollisionConfig.h>
#include<CollisionManager.h>
#include<DxlibInclude.h>
#include "Player.h"

void BossPunchAttack::Attack()
{
	isAttack_ = true;
}

void BossPunchAttack::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(size_ / 2);
	SetShape(shape_);

	userData_ = &tag;
	
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
}

void BossPunchAttack::Update()
{
	if ( isAttack_ )
	{
		time_--;

		if ( !time_ )
		{
			isAttack_ = false;
		}

		shape_->SetCenter({ bossPos_.x + ( bossSize_.x / 2 + size_.x / 2 ) * -dir_ , bossPos_.y });
	}
}

void BossPunchAttack::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);

	DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
		shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
		GetColor(255,255,255),true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void BossPunchAttack::SetBossPos(const Vector2& pos)
{
	bossPos_ = pos;
}

void BossPunchAttack::SetBossSize(const Vector2& size)
{
	bossSize_ = size;
}

void BossPunchAttack::SetTime(int32_t time)
{
	time_ = time;
}

void BossPunchAttack::SetSize(const Vector2& size)
{
	size_ = size;
}

void BossPunchAttack::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossPunchAttack::SetPower(int32_t power)
{
	attackPower_ = power;
}

void BossPunchAttack::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}
