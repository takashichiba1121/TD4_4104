#include "PlayerAttackFist.h"
#include"DxlibInclude.h"
#include"CollisionManager.h"
#include"FlyEnemy.h"
#include"WalkEnemy.h"
#include"BossEnemy.h"
void PlayerAttackFist::Initialize(Vector2* playerPos,Vector2* velocity,bool* direction)
{
	playerPos_ = playerPos;

	direction_ = direction;

	velocity_ = velocity;

	shape_ = new RectShape();

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();

	textureId_ = LoadGraph(std::string("Resources\\Player\\Parts\\normalPunch.png"));

	attackSoundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_arm_punch_Attack.mp3"));

	hitSoundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_arm_punch_Hit.mp3"));
}
void PlayerAttackFist::AttackInit(float pow,float changeCrit,float changeCdmg)
{
	if ( INTERVAL_ <= AttackInterval_ )
	{
		playerPow_ = pow;

		playerCrit_ = changeCrit;

		playerCdmg_ = changeCdmg;

		isAttack_ = true;

		AttackTime_ = 0;

		if ( *direction_ )
		{
			velocity_->x += MOVE_;
		}
		else
		{
			velocity_->x -= MOVE_;
		}

		CollisionEnable();

		PlaySoundMem(attackSoundId_,DX_PLAYTYPE_BACK);
	}
}

void PlayerAttackFist::Attack()
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

		shape_->SetCenter(DrawPos_);

		if ( AttackTime_ > LAST_ATTACK_TIME_ )
		{
			CollisionDisable();
			isAttack_ = false;
			isGiveDamage_ = false;
		}
	}
	else
	{
		AttackInterval_++;
	}
}

void PlayerAttackFist::Draw(Vector2 scroll)
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

void PlayerAttackFist::OnCollision()
{
	if ( GetCollisionInfo().userData && isGiveDamage_ == false )
	{

		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "Enemy" )
		{
			if ( GetRand(1000) <= playerCrit_ * 1000 )
			{
				dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(( playerPow_ * POW_ ) + ( playerPow_ * POW_ * playerCdmg_ ));
			}
			else
			{
				dynamic_cast< BaseEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);
			}
			isGiveDamage_ = true;

			PlaySoundMem(hitSoundId_,DX_PLAYTYPE_BACK);
		}
		if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "BossEnemy" )
		{
			if ( GetRand(1000) <= playerCrit_ * 1000 )
			{
				dynamic_cast< BossEnemy* >( GetCollisionInfo().object )->Damage(( playerPow_ * POW_ ) + ( playerPow_ * POW_ * playerCdmg_ ));
			}
			else
			{
				dynamic_cast< BossEnemy* >( GetCollisionInfo().object )->Damage(playerPow_ * POW_);
			}
			isGiveDamage_ = true;

			PlaySoundMem(hitSoundId_,DX_PLAYTYPE_BACK);
		}
	}
}