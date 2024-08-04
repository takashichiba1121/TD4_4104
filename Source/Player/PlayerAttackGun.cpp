#include "PlayerAttackGun.h"
#include"DxlibInclude.h"
#include"PlayerBulletManager.h"

void PlayerAttackGun::Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;

	velocity_ = velocity;

	textureId_ = LoadGraph(std::string("Resources\\Player\\Parts\\gunArm.png"));

	soundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_arm_gun_Attack.mp3"));
}

void PlayerAttackGun::AttackInit(float pow,float changeCrit,float changeCdmg)
{
	if ( isAttack_ == false )
	{
		isAttack_ = true;

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		if ( *direction_ )
		{
			newBullet->Initialize({ 10,0 },*playerPos_,60,pow*POW_,changeCrit,changeCdmg,PlayerBullet::Type::Normal);

			velocity_->x -= 5;
		}
		else
		{
			newBullet->Initialize({ -10,0 },*playerPos_,60,pow*POW_,changeCrit,changeCdmg,PlayerBullet::Type::Normal);

			velocity_->x += 5;
		}

		PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));

		PlaySoundMem(soundId_,DX_PLAYTYPE_BACK);
	}
}

void PlayerAttackGun::Attack()
{
	if ( isAttack_ )
	{
		if ( *direction_ )
		{
			DrawPos_ = { playerPos_->x + ATTACK_POS_.x + COLISION_SIZE_.x / 2,playerPos_->y + ATTACK_POS_.y };
		}
		else
		{
			DrawPos_ = { playerPos_->x - ATTACK_POS_.x - COLISION_SIZE_.x / 2,playerPos_->y + ATTACK_POS_.y };
		}

		AttackTime_++;

		if ( AttackTime_ > LAST_ATTACK_TIME_ )
		{
			isAttack_ = false;
			AttackTime_ = 0;
		}
	}

}

void PlayerAttackGun::Draw(Vector2 scroll)
{
	if ( isAttack_ )
	{
		if ( *direction_ )
		{
			DrawExtendGraph(scroll.x + DrawPos_.x - COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y - COLISION_SIZE_.y / 2,
				scroll.x + DrawPos_.x + COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y + COLISION_SIZE_.y / 2,
				textureId_,true);
		}
		else
		{
			DrawExtendGraph(scroll.x + DrawPos_.x + COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y - COLISION_SIZE_.y / 2,
				scroll.x + DrawPos_.x - COLISION_SIZE_.x / 2,scroll.y + DrawPos_.y + COLISION_SIZE_.y / 2,
				textureId_,true);
		}
	}
}
