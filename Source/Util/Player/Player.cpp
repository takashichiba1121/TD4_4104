#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"
#include"PlayerAttackFist.h"
#include"PlayerAttackWeapon.h"
#include"PlayerBulletManager.h"
#include"json.hpp"
#include <fstream>

#include"CollisionManager.h"

void Player::Initialize()
{
	hitboxSize_ = { 28,54 };

	drawSize_ = { 34,60 };

	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	ChangeLeftArm("Fist");

	ChangeRightArm("Weapon");

	hp_ = maxHp_;

	name_.tag = "Player";
	userData_ = &name_;

	islive_ = true;

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ hitboxSize_.x / 2,hitboxSize_.y / 2 });

	shape_ = new RectShape();
	shape_->SetRadius(hitboxSize_ / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);

	textureId_ = LoadGraph("Resources/Player/PlayerStand.png");

	ChangeLeg("Normal");
}

void Player::Update()
{
	if ( DamageInterval_ < DAMAGE_INTERVAL_MAX_ )
	{
		DamageInterval_++;
	}

	leg_->Move(GetOnDir() & 0b1 << OnDir::BOTTOM,leftArm_->IsAttack() || rightArm_->IsAttack());

	Attack();

	PlayerBulletManager::Instance()->Update();

	SetMapChipSpeed(velocity_);

	shape_->SetCenter(pos_);

#ifdef _DEBUG

	ImGui::Begin("PlayerSituation");

	ImGui::Text("vec:%f,%f",velocity_.x,velocity_.y);

	ImGui::Text("HP:%d",hp_);

	ImGui::End();

#endif
}

void Player::Attack()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && leftArm_ != nullptr&&!rightArm_->IsAttack() )
	{
		leftArm_->AttackInit(changePow_);
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_X) && rightArm_ != nullptr && !leftArm_->IsAttack() )
	{
		rightArm_->AttackInit(changePow_);
	}

	if ( leftArm_ != nullptr )
	{
		leftArm_->Attack();
	}

	if ( rightArm_ != nullptr )
	{
		rightArm_->Attack();
	}
}
void Player::Damage(int32_t Damage)
{
	if ( DamageInterval_ >= DAMAGE_INTERVAL_MAX_ )
	{
		hp_ -= Damage * changeDef_;

		DamageInterval_ = 0;
	}
}
void Player::ChangeLeftArm(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		leftArm_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		leftArm_ = std::make_unique<PlayerAttackWeapon>();
	}

	leftArm_->Initialize(&pos_,&velocity_,&direction_);
}

void Player::ChangeRightArm(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		rightArm_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		rightArm_ = std::make_unique<PlayerAttackWeapon>();
	}

	rightArm_->Initialize(&pos_,&velocity_ ,&direction_);
}

void Player::ChangeLeg(std::string legName)
{
	if ( legName == "Normal" )
	{
		leg_ = std::make_unique<PlayerLegNormal>();
	}

	leg_->Initialize(&velocity_,&direction_,&changeAcl_);
}

bool Player::AddSpd(int32_t spd)
{
	changeAcl_ += float(spd) / 100.0f;//パーセントを実数値に戻す

	return true;
}

bool Player::AddPow(int32_t pow)
{
	changePow_ += float(pow) / 100.0f;

	return true;
}

bool Player::AddDef(int32_t def)
{
	changeDef_ += float(def) / 100.0f;
	return true;
}

bool Player::AddMaxHp(int32_t maxHp)
{
	changeMaxHp_ += float(maxHp) / 100.0f;

	return true;
}
bool Player::AddCrit(float Crit)
{
	changeCrit_ += Crit;

	return true;
}
bool Player::AddCdmg(int32_t Cdmg)
{
	changeCdmg_ += float(Cdmg) / 100.0f;

	return true;
}
bool Player::SubSpd(int32_t spd)
{
	if ( changeAcl_ - float(spd) / 100.0f <= 0 )
	{
		return false;
	}
	changeAcl_ -= float(spd) / 100.0f;//パーセントを実数値に戻す

	return true;
}

bool Player::SubPow(int32_t pow)
{
	if ( changePow_ - float(pow) / 100.0f <= 0 )
	{
		return false;
	}
	changePow_ -= float(pow) / 100.0f;

	return true;
}

bool Player::SubDef(int32_t def)
{
	if ( changeDef_ - float(def) / 100.0f <= 0 )
	{
		return false;
	}
	changeDef_ -= float(def) / 100.0f;
	return true;
}

bool Player::SubMaxHp(int32_t maxHp)
{
	if ( changeMaxHp_ - float(maxHp) / 100.0f <= 0 )
	{
		return false;
	}
	changeMaxHp_ -= float(maxHp) / 100.0f;

	return true;
}

bool Player::SubCrit(int32_t Crit)
{
	changeCrit_ -= Crit;

	return true;
}

bool Player::SubCdmg(int32_t Cdmg)
{
	changeCdmg_ -= float(Cdmg) / 100.0f;

	return true;
}

void Player::Draw()
{
	PlayerBulletManager::Instance()->Draw();

	float leftPos = pos_.x - drawSize_.x / 2;
	float rightPos = pos_.x + drawSize_.x / 2;
	float upPos = pos_.y - drawSize_.y / 2;
	float downPos = pos_.y + drawSize_.y / 2;
	if ( DamageInterval_ % 2 == 0 )
	{

		if ( direction_ )
		{
			DrawExtendGraph(leftPos,upPos,rightPos,downPos,textureId_,TRUE);
		}
		else
		{
			DrawExtendGraph(rightPos,upPos,leftPos,downPos,textureId_,TRUE);
		}
	}

	if ( leftArm_ != nullptr )
	{
		leftArm_->Draw();
	}

	if ( rightArm_ != nullptr )
	{
		rightArm_->Draw();
	}

	DrawFormatString(0,GameConfig::GetWindowHeight() - 20,0xffffff,"PlayerHP:%d/%d",hp_,maxHp_);
}