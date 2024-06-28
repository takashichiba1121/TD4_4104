#include "PlayerAttackWeapon.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"

void PlayerAttackWeapon::Initialize(Vector2* playerPos,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;
}

void PlayerAttackWeapon::AttackInit(float pow)
{
	if ( isAttack_ == false )
	{
		isAttack_ = true;

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		if ( *direction_ )
		{
			newBullet->Initialize({ 10,0 },*playerPos_,60,pow);
		}
		else
		{
			newBullet->Initialize({ -10,0 },*playerPos_,60,pow);
		}

		PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
	}
}

void PlayerAttackWeapon::Attack()
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

void PlayerAttackWeapon::Draw()
{

}
