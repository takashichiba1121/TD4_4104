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
#include"PlayerLegCerberus.h"
#include"json.hpp"
#include <fstream>

#include"CollisionManager.h"

void Player::Initialize()
{
	hitboxSize_ = { 60,128 };

	drawSize_ = { 128,128 };

	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	leftArm_ = std::make_unique<PlayerAttackFist>();

	leftArm_->Initialize(&pos_,&velocity_,&direction_);

	rightArm_ = std::make_unique<PlayerAttackFist>();

	rightArm_->Initialize(&pos_,&velocity_,&direction_);

	hp_ = MAX_HP_ * changeMaxHp_;

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

	leg_ = std::make_unique<PlayerLegNormal>();

	leg_->Initialize(&velocity_,&direction_,&changeSpd_);

	//Item item;

	//item.power = 50;

	//item.statusName = "ATK";

	//ItemGet(item);

	circelShape_ = std::make_unique<CircleShape>();

	circelShape_->SetRadius(hitboxSize_.y);

	//ChangeLeg("Fenrir",0);

	//ChangeLeftArm("Cerberus",0);

	//ChangeRightArm("Gun",0);
	//ChangeEye("Clairvoyance",0);

	hpBerTextureId_ = LoadGraph(std::string("Resources\\UI\\UI_HP_Bar.png"));

	hpBerBackgroundTextureId_ = LoadGraph(std::string("Resources\\UI\\UI_HP_Background.png"));

	statusFrameTextureId_ = LoadGraph(std::string("Resources\\UI\\UI_status_frame.png"));
}

void Player::Update()
{
	if ( isPowerUp_ )
	{
		PowerUp();
	}
	else
	{
		if ( DamageInterval_ < DAMAGE_INTERVAL_MAX_ )
		{
			DamageInterval_++;
		}

		/*if (Input::Instance()->TriggerKey(KEY_INPUT_1))
		{
			selectItems_ = 1;
		}
		if (Input::Instance()->TriggerKey(KEY_INPUT_2))
		{
			selectItems_ = 2;
		}
		if (Input::Instance()->TriggerKey(KEY_INPUT_3))
		{
			selectItems_ = 3;
		}

		if (Input::Instance()->TriggerKey(KEY_INPUT_RETURN))
		{
			UseItem();
		}*/

		leg_->Move(GetOnDir() & 0b1 << OnDir::BOTTOM,leftArm_->IsAttack() || rightArm_->IsAttack(),pos_,changePow_);

		Attack();

		PlayerBulletManager::Instance()->Update();

		SetMapChipSpeed(velocity_);

		shape_->SetCenter(pos_);

		circelShape_->SetCenter(pos_);
	}

	powerUpText_ = false;

#ifdef _DEBUG

	ImGui::Begin("PlayerSituation");

	ImGui::Text("vec:%d,%d",Input::Instance()->PadX(),Input::Instance()->PadY());
	ImGui::End();

#endif
}

void Player::Attack()
{
	if ( !powerUpText_ )
	{
		if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) || Input::Instance()->TriggerPadKey(PAD_INPUT_3) && leftArm_ != nullptr && !rightArm_->IsAttack() )
		{
			leftArm_->AttackInit(changePow_,changeCrit_,changeCdmg_);
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_X) || Input::Instance()->TriggerPadKey(PAD_INPUT_4) && rightArm_ != nullptr && !leftArm_->IsAttack() )
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
}
void Player::Damage(int32_t damage)
{
	if ( DamageInterval_ >= DAMAGE_INTERVAL_MAX_ && !leg_->IsEvasionRoll() )
	{
		if ( damage - ( changeDef_ * leg_->GetDef() ) >= 5 )
		{

			hp_ -= damage - ( changeDef_ * leg_->GetDef() );
		}
		else
		{
			hp_ -= 5;
		}
		DamageInterval_ = 0;
	}
}
void Player::IventDamage(int32_t damage)
{
	if ( hp_ - damage >= 0 )
	{
		hp_ -= damage;
	}
	else
	{
		hp_ = 1;
	}
}
bool Player::ChangeLeftArm(std::string attackName,uint32_t cost)
{
	if ( nowCost_ + cost - leftArm_->cost > MAX_COST_ * changeMaxCost_ )
	{
		return false;
	}

	if ( attackName == "Fist" )
	{
		leftArm_ = std::make_unique<PlayerAttackFist>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Fist;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Gun" )
	{
		leftArm_ = std::make_unique<PlayerAttackGun>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Gun;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Cerberus" )
	{
		leftArm_ = std::make_unique<PlayerAttackCerberus>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Cerberus;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Fenrir" )
	{
		leftArm_ = std::make_unique<PlayerAttackFenrir>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Fenrir;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Mars" )
	{
		leftArm_ = std::make_unique<PlayerAttackMars>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Mars;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Spider" )
	{
		leftArm_ = std::make_unique<PlayerAttackSpider>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Spider;
		nowPartTag_.leftAtaackName = attackName;
	}
	else if ( attackName == "Vine" )
	{
		leftArm_ = std::make_unique<PlayerAttackVine>();
		nowPartTag_.leftAtaackTag = PlayerAttackTags::Vine;
		nowPartTag_.leftAtaackName = attackName;
	}
	else
	{
		return false;
	}

	nowCost_ -= leftArm_->cost;

	leftArm_->cost = cost;

	nowCost_ += cost;

	leftArm_->Initialize(&pos_,&velocity_,&direction_);
	return true;
}

bool Player::ChangeRightArm(std::string attackName,uint32_t cost)
{
	if ( nowCost_ + cost - rightArm_->cost > MAX_COST_ * changeMaxCost_ )
	{
		return false;
	}

	if ( attackName == "Fist" )
	{
		rightArm_ = std::make_unique<PlayerAttackFist>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Fist;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Gun" )
	{
		rightArm_ = std::make_unique<PlayerAttackGun>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Gun;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Cerberus" )
	{
		rightArm_ = std::make_unique<PlayerAttackCerberus>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Cerberus;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Fenrir" )
	{
		rightArm_ = std::make_unique<PlayerAttackFenrir>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Fenrir;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Mars" )
	{
		rightArm_ = std::make_unique<PlayerAttackMars>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Mars;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Spider" )
	{
		rightArm_ = std::make_unique<PlayerAttackSpider>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Spider;
		nowPartTag_.rightAtaackName = attackName;
	}
	else if ( attackName == "Vine" )
	{
		rightArm_ = std::make_unique<PlayerAttackVine>();
		nowPartTag_.rightAtaackTag = PlayerAttackTags::Vine;
		nowPartTag_.rightAtaackName = attackName;
	}
	else
	{
		return false;
	}

	nowCost_ -= rightArm_->cost;

	rightArm_->cost = cost;

	nowCost_ += cost;

	rightArm_->Initialize(&pos_,&velocity_,&direction_);

	return true;
}

bool Player::ChangeLeg(std::string legName,uint32_t cost)
{
	if ( nowCost_ + cost - leg_->cost > MAX_COST_ * changeMaxCost_ )
	{
		return false;
	}

	if ( legName == "Normal" )
	{
		leg_ = std::make_unique<PlayerLegNormal>();
		nowPartTag_.legTag = PlayerLegTags::Normal;
		nowPartTag_.legName = legName;
	}
	if ( legName == "Cerberus" )
	{
		leg_ = std::make_unique<PlayerLegCerberus>();
		nowPartTag_.legTag = PlayerLegTags::Cerberus;
		nowPartTag_.legName = legName;
	}
	if ( legName == "Fenrir" )
	{
		leg_ = std::make_unique<PlayerLegFenrir>();
		nowPartTag_.legTag = PlayerLegTags::Fenrir;
		nowPartTag_.legName = legName;
	}

	nowCost_ -= leg_->cost;

	leg_->cost = cost;

	nowCost_ += cost;

	leg_->Initialize(&velocity_,&direction_,&changeSpd_);

	return true;
}

bool Player::ChangeEye(std::string eyeName,uint32_t cost)
{
	if ( nowCost_ + cost - nowEyeCost_ > MAX_COST_ * changeMaxCost_ )
	{
		return false;
	}

	if ( eyeName == "Normal" )
	{
		nowPartTag_.eyeTag = PlayerEyeTags::Normal;
		nowPartTag_.eyeName = eyeName;
	}
	if ( eyeName == "Clairvoyance" )
	{
		nowPartTag_.eyeTag = PlayerEyeTags::Clairvoyance;
		nowPartTag_.eyeName = eyeName;
	}
	if ( eyeName == "Curse" )
	{
		nowPartTag_.eyeTag = PlayerEyeTags::Curse;
		nowPartTag_.eyeName = eyeName;
	}

	nowCost_ -= nowEyeCost_;

	nowEyeCost_ = cost;

	nowCost_ += cost;

	return true;
}

bool Player::ChangeMouth(std::string mouthName,uint32_t cost)
{
	if ( nowCost_ + cost - nowMouthCost_ > MAX_COST_ * changeMaxCost_ )
	{
		return false;
	}

	if ( mouthName == "Normal" )
	{
		nowPartTag_.mouthTag = PlayerMouthTags::Normal;
		nowPartTag_.mouthName = mouthName;
	}
	if ( mouthName == "Clairvoyance" )
	{
		nowPartTag_.mouthTag = PlayerMouthTags::Soul;
		nowPartTag_.mouthName = mouthName;
	}

	nowCost_ -= nowMouthCost_;

	nowMouthCost_ = cost;

	nowCost_ += cost;

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

	changeMaxHp_ += maxHp / 100.0f;

	hp_ += ( MAX_HP_ * changeMaxHp_ ) - nowMaxHp;

	return true;
}
bool Player::AddCrit(float Crit)
{
	changeCrit_ += float(Crit) / 100.0f;;

	return true;
}
bool Player::AddCdmg(int32_t Cdmg)
{
	changeCdmg_ += float(Cdmg) / 100.0f;

	return true;
}

void Player::AddMaxCost(int32_t AddMaxCost)
{
	changeMaxCost_ += AddMaxCost;
}

bool Player::SubSpd(int32_t spd)
{
	if ( changeSpd_ - float(spd) / 100.0f <= 0.75f )
	{
		changeSpd_ = 7.5f;
		return false;
	}
	changeSpd_ -= float(spd) / 100.0f;//パーセントを実数値に戻す

	return true;
}

bool Player::SubPow(int32_t pow)
{
	if ( changePow_ - float(pow) / 100.0f <= 0.5f )
	{
		changePow_ = 0.5f;
		return false;
	}
	changePow_ -= float(pow) / 100.0f;

	return true;
}

bool Player::SubDef(int32_t def)
{
	if ( changeDef_ - float(def) / 100.0f <= 0.1f )
	{
		changeDef_ = 0.1f;
		return false;
	}
	changeDef_ -= float(def) / 100.0f;
	return true;
}

bool Player::SubMaxHp(int32_t maxHp)
{
	if ( changeMaxHp_ - maxHp / 100.0f <= 0.1f )
	{
		changeMaxHp_ = 0.1f;
		return false;
	}
	changeMaxHp_ -= maxHp / 100.0f;

	if ( hp_ >= MAX_HP_ * changeMaxHp_ )
	{
		hp_ = MAX_HP_ * changeMaxHp_;
	}

	return true;
}

bool Player::SubCrit(int32_t Crit)
{
	if ( changeCrit_ - float(Crit) / 100.0f <= 0.01f )
	{
		changeCrit_ = 0.1f;
		return false;
	}
	changeCrit_ -= float(Crit) / 100.0f;
	return true;
}

bool Player::SubCdmg(int32_t Cdmg)
{
	if ( changeCdmg_ - float(Cdmg) / 100.0f <= 0.1f )
	{
		changeCdmg_ = 0.1f;
		return false;
	}
	changeCdmg_ -= float(Cdmg) / 100.0f;
	return true;
}

void Player::Draw(Vector2 scroll)
{
	PlayerBulletManager::Instance()->Draw(scroll);

	if ( DamageInterval_ % 5 == 0 )
	{
		leg_->Draw(pos_,drawSize_,scroll);
	}

	if ( leftArm_ != nullptr )
	{
		leftArm_->Draw(scroll);
	}

	if ( rightArm_ != nullptr )
	{
		rightArm_->Draw(scroll);
	}
	DrawExtendGraph(30,GameConfig::GetWindowHeight() - 40,
		MAX_HP_ * changeMaxHp_ * 2,GameConfig::GetWindowHeight() - 8,
		hpBerBackgroundTextureId_,false);
	DrawExtendGraph(30,GameConfig::GetWindowHeight() - 40,
		hp_ * 2,GameConfig::GetWindowHeight() - 8,
		hpBerTextureId_,false);
	DrawFormatString(40,GameConfig::GetWindowHeight() - 30,0xffffff,"%d/%d",hp_,int(MAX_HP_ * changeMaxHp_));


	if ( powerUpText_ && isPowerUp_ == false )
	{
		DrawFormatString(pos_.x,pos_.y - drawSize_.y + 40,0xffffff,"Push to KEY X");
	}
	if ( isPowerUp_ )
	{
		DrawExtendGraph(20,20,
		148,170,
		statusFrameTextureId_,false);

		std::string ATK = std::to_string(int(changePow_ * 100));

		ATK += "%";

		ATK = "ATK " + ATK;

		std::string DEF = std::to_string(int(changeDef_ * 100));

		DEF += "%";

		DEF = "DEF " + DEF;

		std::string SPD = std::to_string(int(changeSpd_ * 100));

		SPD += "%";

		SPD = "SPD " + SPD;

		std::string MAXHP = std::to_string(int(changeMaxHp_*100));

		MAXHP += "%";

		MAXHP = "HP " + MAXHP;

		std::string CRIT = std::to_string(int(changeCrit_ * 100));

		CRIT += "%";

		CRIT = "CRIT " + CRIT;

		std::string CDMG = std::to_string(int(changeCdmg_ * 100));

		CDMG += "%";

		CDMG = "CDMG " + CDMG;


		DrawString(30,30,MAXHP.c_str(),GetColor(255,255,255));

		DrawString(30,53,ATK.c_str(),GetColor(255,255,255));

		DrawString(30,76,DEF.c_str(),GetColor(255,255,255));

		DrawString(30,99,SPD.c_str(),GetColor(255,255,255));

		DrawString(30,122,CRIT.c_str(),GetColor(255,255,255));

		DrawString(30,145,CDMG.c_str(),GetColor(255,255,255));
	}
	if ( isChangeParts_ )
	{
		DrawExtendGraph(20,20,
						148,73,
		statusFrameTextureId_,false);

		DrawFormatString(30,30,GetColor(255,255,255),"NowCost %d",nowCost_);

		DrawFormatString(30,53,GetColor(255,255,255),"MAXCost %d",MAX_COST_+ changeMaxCost_);
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
				if ( hp_ > MAX_HP_ * changeMaxHp_ )
				{
					hp_ = MAX_HP_ * changeMaxHp_;
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
	if ( Input::Instance()->TriggerKey(KEY_INPUT_LEFT) || Input::Instance()->TriggerKey(KEY_INPUT_A) || Input::Instance()->TriggerPadKey(PAD_INPUT_5) )
	{
		if ( powerUpNum_ == 0 )
		{
			powerUpNum_ = 2;
		}
		else
		{
			powerUpNum_--;
		}
	}
	if ( Input::Instance()->TriggerKey(KEY_INPUT_RIGHT) || Input::Instance()->TriggerKey(KEY_INPUT_D) || Input::Instance()->TriggerPadKey(PAD_INPUT_6) )
	{
		powerUpNum_++;
		if ( powerUpNum_ >= 3 )
		{
			powerUpNum_ = 0;
		}
	}
	return powerUpNum_;
}

void Player::EndPowerUp()
{
	isPowerUp_ = false;
}

void Player::EndChangeParts()
{
	isChangeParts_ = false;
}

void Player::Reset()
{
	isDealed_ = false;
}

void Player::OnCollision()
{
	if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "PowerUp" && isDealed_ == false )
	{
		powerUpText_ = true;
		if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) || Input::Instance()->TriggerPadKey(PAD_INPUT_3) )
		{
			isPowerUp_ = true;

			isDealed_ = true;
		}
	}
	if ( static_cast< ObjectUserData* >( GetCollisionInfo().userData )->tag == "Parts" && isDealed_ == false )
	{
		powerUpText_ = true;
		if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) || Input::Instance()->TriggerPadKey(PAD_INPUT_3) )
		{
			isChangeParts_ = true;

			isDealed_ = true;
		}
	}
}

bool Player::CheckHavePart(PartsName partType,std::string partName)
{
	switch ( partType )
	{
	case ARM:
		if ( nowPartTag_.leftAtaackName == partName )
		{
			return true;
		}
		else if ( nowPartTag_.rightAtaackName == partName )
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case LEG:
		if ( nowPartTag_.legName == partName )
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case EYE:
		if ( nowPartTag_.eyeName == partName )
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case MOUTH:
		if ( nowPartTag_.mouthName == partName )
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		break;
	}

	return false;
}

void Player::SoulMouth()
{
	if ( nowPartTag_.mouthTag == PlayerMouthTags::Soul )
	{
		hp_ += 10;

		if ( hp_ > MAX_HP_ * changeMaxHp_ )
		{
			hp_ = MAX_HP_ * changeMaxHp_;
		}
	}
}

void Player::Heel(uint32_t heel)
{
	hp_ += heel;

	if ( hp_ > MAX_HP_ * changeMaxHp_ )
	{
		hp_ = MAX_HP_ * changeMaxHp_;
	}
}
