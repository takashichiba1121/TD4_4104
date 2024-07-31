#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"
#include"PlayerAttackFist.h"
#include"PlayerAttackGun.h"
#include"PlayerAttackCerberus.h"
#include"PlayerAttackFenrir.h"
#include"PlayerAttackMars.h"
#include"PlayerAttackVine.h"
#include"PlayerAttackSpider.h"
#include"PlayerBulletManager.h"
#include"PlayerLegNormal.h"
#include"PlayerLegFenrir.h"
#include"json.hpp"
#include <fstream>

#include"CollisionManager.h"

void Player::Initialize()
{
	hitboxSize_ = { 64,128 };

	drawSize_ = { 128,128 };

	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	leftArm_ = std::make_unique<PlayerAttackFist>();

	leftArm_->Initialize(&pos_,&velocity_,&direction_);

	rightArm_ = std::make_unique<PlayerAttackFenrir>();

	rightArm_->Initialize(&pos_,&velocity_,&direction_);

	hp_ = MAX_HP_;

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

	leg_ = std::make_unique<PlayerLegFenrir>();

	leg_->Initialize(&velocity_,&direction_,&changeSpd_);

	Item item;

	item.power = 50;

	item.statusName = "ATK";

	ItemGet(item);

	circelShape = std::make_unique<CircleShape>();

	circelShape->SetRadius(hitboxSize_.y);
}

void Player::Update()
{
	powerUpText = false;

	if ( isPowerUp )
	{
		PowerUp();
	}
	else
	{
		if ( DamageInterval_ < DAMAGE_INTERVAL_MAX_ )
		{
			DamageInterval_++;
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_1) )
		{
			selectItems_ = 1;
		}
		if ( Input::Instance()->TriggerKey(KEY_INPUT_2) )
		{
			selectItems_ = 2;
		}
		if ( Input::Instance()->TriggerKey(KEY_INPUT_3) )
		{
			selectItems_ = 3;
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_RETURN) )
		{
			UseItem();
		}

		leg_->Move(GetOnDir() & 0b1 << OnDir::BOTTOM,leftArm_->IsAttack() || rightArm_->IsAttack(),pos_,changePow_);

		Attack();

		PlayerBulletManager::Instance()->Update();

		SetMapChipSpeed(velocity_);

		shape_->SetCenter(pos_);

		circelShape->SetCenter(pos_);
	}

#ifdef _DEBUG

	ImGui::Begin("PlayerSituation");

	ImGui::Text("vec:%f,%f",velocity_.x,velocity_.y);

	ImGui::Text("HP:%d",hp_);

	ImGui::Text("MaxHP:%d",MAX_HP_);

	ImGui::Text("Pow:%1.2f",changePow_);

	ImGui::Text("Acl:%1.2f",changeSpd_);

	ImGui::Text("Def:%1.2f",changeDef_);

	ImGui::Text("Crit:%1.2f",changeCrit_);

	ImGui::Text("Cdmg:%1.2f",changeCdmg_);

	ImGui::Text("cost:%d",nowCost);

	ImGui::End();

#endif
}

void Player::Attack()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && leftArm_ != nullptr && !rightArm_->IsAttack() )
	{
		leftArm_->AttackInit(changePow_,changeCrit_,changeCdmg_);
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_X) && rightArm_ != nullptr && !leftArm_->IsAttack() )
	{
		rightArm_->AttackInit(changePow_,changeCrit_,changeCdmg_);
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
		hp_ -= Damage - (changeDef_*DEF_);

		DamageInterval_ = 0;
	}
}
bool Player::ChangeLeftArm(std::string attackName,uint32_t cost)
{
	if ( nowCost + cost - leftArm_->cost > 100 )
	{
		return false;
	}

	if ( attackName == "Fist" )
	{
		leftArm_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		leftArm_ = std::make_unique<PlayerAttackGun>();
	}

	leftArm_->cost = cost;

	leftArm_->Initialize(&pos_,&velocity_,&direction_);
	return true;
}

bool Player::ChangeRightArm(std::string attackName,uint32_t cost)
{
	if ( nowCost + cost - rightArm_->cost > 100 )
	{
		return false;
	}

	if ( attackName == "Fist" )
	{
		rightArm_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		rightArm_ = std::make_unique<PlayerAttackGun>();
	}

	rightArm_->cost = cost;

	rightArm_->Initialize(&pos_,&velocity_,&direction_);

	return true;
}

bool Player::ChangeLeg(std::string legName,uint32_t cost)
{
	if ( nowCost + cost - leg_->cost > 100 )
	{
		return false;
	}

	if ( legName == "Normal" )
	{
		leg_ = std::make_unique<PlayerLegNormal>();
	}

	leg_->cost = cost;

	leg_->Initialize(&velocity_,&direction_,&changeSpd_);

	return true;
}

bool Player::AddSpd(int32_t spd)
{
	changeSpd_ += float(spd) / 100.0f;//パーセントを実数値に戻す

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
	uint32_t nowMaxHp = MAX_HP_ * changeMaxHp_;

	changeMaxHp_ += maxHp;

	hp_ += ( MAX_HP_ * changeMaxHp_ ) - nowMaxHp;

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
	if ( changeSpd_ - float(spd) / 100.0f <= 0 )
	{
		return false;
	}
	changeSpd_ -= float(spd) / 100.0f;//パーセントを実数値に戻す

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
	if ( changeMaxHp_- maxHp <= 0 )
	{
		return false;
	}
	changeMaxHp_ -= maxHp;

	if ( hp_ >= MAX_HP_* changeMaxHp_ )
	{
		hp_ = MAX_HP_* changeMaxHp_;
	}

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

	if ( DamageInterval_ % 2 == 0 )
	{
		leg_->Draw(pos_,drawSize_);
	}

	if ( leftArm_ != nullptr )
	{
		leftArm_->Draw();
	}

	if ( rightArm_ != nullptr )
	{
		rightArm_->Draw();
	}

	DrawFormatString(0,GameConfig::GetWindowHeight() - 20,0xffffff,"PlayerHP:%d/%d",hp_,MAX_HP_);

	if (powerUpText&&isPowerUp==false )
	{
		DrawFormatString(pos_.x,pos_.y-drawSize_.y+40,0xffffff,"Push to KEY Z",hp_,MAX_HP_);
	}
}

bool Player::ItemGet(Item newItem)
{
	if ( items_.size() < 3 )
	{
		items_.push_back(newItem);

		return true;
	}
	return false;
}

void Player::UseItem()
{
	if ( items_.size() == 0 && items_.size() < selectItems_ )
	{
		return;
	}

	std::list<Item>::iterator  itr = items_.begin();
	uint16_t num = 0;

	while ( itr != items_.end() )
	{
		num++;
		if ( num == selectItems_ )
		{
			if ( itr->statusName == "HP" )
			{
				hp_ += itr->power;
				if ( hp_ > MAX_HP_ )
				{
					hp_ = MAX_HP_;
				}
			}
			if ( itr->statusName == "ATK" )
			{
				changePow_ += float(itr->power) / 100.0f;
			}
			if ( itr->statusName == "DEF" )
			{
				changeDef_ += float(itr->power) / 100.0f;
			}
			if ( itr->statusName == "SPD" )
			{
				changeSpd_ += float(itr->power) / 100.0f;
			}
			if ( itr->statusName == "CRIT" )
			{
				changeCrit_ += float(itr->power) / 100.0f;
			}
			if ( itr->statusName == "CDMG" )
			{
				changeCdmg_ += float(itr->power) / 100.0f;
			}
			items_.erase(itr);
			break;
		}

		itr++;
	}
}

uint32_t Player::PowerUp()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_LEFT) || Input::Instance()->TriggerKey(KEY_INPUT_A) )
	{
		if ( powerUpNum == 0 )
		{
			powerUpNum = 2;
		}
		else
		{
			powerUpNum--;
		}
	}
	if ( Input::Instance()->TriggerKey(KEY_INPUT_RIGHT) || Input::Instance()->TriggerKey(KEY_INPUT_D) )
	{
		powerUpNum++;
		if ( powerUpNum >= 3 )
		{
			powerUpNum = 0;
		}
	}
	return powerUpNum;
}

void Player::EndPowerUp()
{
	isPowerUp = false;
}

void Player::Reset()
{
	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;
	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;
	isDealed_ = false;
}

void Player::OnCollision()
{
	if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "PowerUpCave"&&isDealed_==false )
	{
		powerUpText = true;
		if (Input::Instance()->TriggerKey(KEY_INPUT_Z) )
		{
			dynamic_cast< PowerUpCave* >( GetCollisionInfo().object )->SetPriducts();

			isPowerUp = true;

			isDealed_ = true;
		}
	}
}
