#include "BossChargeAttack.h"
#include <Util.h>
#include<CollisionConfig.h>
#include<CollisionManager.h>
#include<DxlibInclude.h>
#include<Player.h>

void BossChargeAttack::Attack()
{
	isAttack_ = true;
	velocity_.x = dir_;
	velocity_.y = 0;
	CollisionEnable();
}

void BossChargeAttack::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(size_ / 2);
	SetShape(shape_);

	userData_ = &tag;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();
	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ size_.x / 2,size_.y / 2 });
}

void BossChargeAttack::Update()
{
	if ( isAttack_ )
	{
		time_--;

		if ( 0 > time_ )
		{
			isAttack_ = false;
			CollisionDisable();
		}

		speed_ = InQuad(SPEED,0,TIME,time_);

		SetMapChipSpeed({ velocity_ * speed_ });
		shape_->SetCenter(pos_);
	}
}

void BossChargeAttack::Draw()
{
	DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
	shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
	GetColor(255,255,255),true);
}

void BossChargeAttack::SetBossPos(const Vector2& pos)
{
	pos_ = pos;
}

void BossChargeAttack::SetTime(int32_t time)
{
	TIME = time_ = time;
}

void BossChargeAttack::SetSize(const Vector2& size)
{
	size_ = size_;
}

void BossChargeAttack::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossChargeAttack::SetPower(int32_t power)
{
	attackPower_ = power;
}

bool BossChargeAttack::IsAttack() const
{
	return isAttack_;
}

void BossChargeAttack::SetSpeed(float speed)
{
	SPEED = speed_ = speed;
}

float BossChargeAttack::GetSpeed() const
{
	return  velocity_.x * speed_;
}

void BossChargeAttack::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}
