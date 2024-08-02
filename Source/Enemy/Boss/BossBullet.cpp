#include "BossBullet.h"
#include<Player.h>
#include<CollisionConfig.h>
#include<CollisionManager.h>
#include<DxlibInclude.h>

void BossBullet::Attack()
{
	isAttack_ = true;
	velocity_.x = -dir_;
	velocity_.y = 0;
	shape_->SetCenter(pos_);
	CollisionEnable();
	MapChipObjectEnable();
}

void BossBullet::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(size_ / 2);
	SetShape(shape_);

	userData_ = &tag;

	CollisionDisable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ size_.x / 2,size_.y / 2 });

	MapChipObjectEnable();
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	MapChipObjectDisable();
}

void BossBullet::Update()
{
	if ( isAttack_ )
	{
		if ( time_ == 0 || GetOnDir() & OnDir::LEFT || GetOnDir() & OnDir::RIGHT )
		{
			isAttack_ = false;
			time_ = TIME;

			CollisionDisable();
			MapChipObjectDisable();
		}
		else
		{
			time_--;

			if ( pos_.x >= 270 )
			{
				int b = 0; b++;
			}


			SetMapChipSpeed({ velocity_ * speed_ });
			shape_->SetCenter(pos_);
		}
	}

}

void BossBullet::Draw()
{
	if ( isAttack_ )
	{
		DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
				shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
				GetColor(255,255,255),true);
	}

}

void BossBullet::DebugDraw()
{
	if ( isAttack_ )
	{
		DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
			shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
			GetColor(255,255,255),true);
	}


}

void BossBullet::SetBossPos(const Vector2& pos)
{
	pos_ = pos;
}

void BossBullet::SetTime(int32_t time)
{
	time_ = TIME = time;
}

void BossBullet::SetSize(const Vector2& size)
{
	size_ = size;
}

void BossBullet::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossBullet::SetPower(int32_t power)
{
	attackPower_ = power;
}

bool BossBullet::IsAttack() const
{
	return isAttack_;
}

void BossBullet::SetSpeed(float speed)
{
	speed_ = speed;
}

void BossBullet::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}
