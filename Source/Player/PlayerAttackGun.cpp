#include "PlayerAttackGun.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"

void PlayerAttackGun::Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;

	velocity_ = velocity;
}

void PlayerAttackGun::AttackInit(float pow)
{
	if ( isAttack_ == false )
	{
		isAttack_ = true;

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		if ( *direction_ )
		{
			newBullet->Initialize({ 10,0 },*playerPos_,60,pow*POW_);

			velocity_->x -= 5;
		}
		else
		{
			newBullet->Initialize({ -10,0 },*playerPos_,60,pow*POW_);

			velocity_->x += 5;
		}

		PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
	}
}

void PlayerAttackGun::Attack()
{
	if ( isAttack_ )
	{
		AttackTime_++;

		if ( AttackTime_ > LAST_ATTACK_TIME_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackGun::Draw()
{

}
