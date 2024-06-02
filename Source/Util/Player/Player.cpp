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

void Player::Initialze()
{
	Load();

	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	ChangeAttackZ("Fist");

	ChangeAttackX("Weapon");
}

void Player::Update()
{

	if ( attackInterval_ > 0 )
	{
		attackInterval_--;
	}

	if ( attackZ_->GetAttack() == false&& attackX_->GetAttack() == false )
	{
		Move();
	}

	Attack();

#ifdef _DEBUG

	ImGui::Begin("PlayerStatus");

	if ( ImGui::Button("Load") )
	{
		Load();
	}

	ImGui::Text("Jump");

	ImGui::SliderFloat("JumpInitialVelocity",&jumpInitialVelocity_,0.0f,-20.0f,"%2.0f");

	ImGui::SliderFloat("JumpAcceleration",&jumpAcceleration_,0.0f,20.0f,"%2.1f");

	ImGui::SliderFloat("GravityAcceleration",&gravityAcceleration_,0.0f,5.0f,"%1.1f");

	ImGui::Text("Move");

	ImGui::SliderFloat("Acceleration",&acceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirAcceleration",&airAcceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("Deccelaration",&deccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirDeccelaration_",&airDeccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("TopSpeed",&topSpeed_,0.0f,200.0f,"%3.0f");

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		direction_ = false;
		if ( speed_ > -topSpeed_*changeSpd_)
		{
			if ( onGround_ )
			{
				speed_ -= ( airAcceleration_ * changeSpd_ ) /GameConfig::GetGameConfig()->fps;
			}
			else
			{
				speed_ -= (acceleration_*changeSpd_) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			speed_ = -topSpeed_ * changeSpd_;
		}
	}
	else if ( speed_ < 0 )
	{
		if ( onGround_ )
		{
			speed_ += airDeccelaration_ /GameConfig::GetGameConfig()->fps;
		}
		else
		{
			speed_ += deccelaration_ /GameConfig::GetGameConfig()->fps;
		}
		if ( speed_ > 0 )
		{
			speed_ = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		direction_ = true;
		if ( speed_ < topSpeed_ *changeSpd_ )
		{
			if ( onGround_ )
			{
				speed_ += ( airAcceleration_ * changeSpd_ ) / GameConfig::GetGameConfig()->fps;
			}
			else
			{
				speed_ += ( acceleration_ * changeSpd_ ) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			speed_ = topSpeed_ * changeSpd_;
		}
	}
	else if ( speed_ > 0 )
	{
		if ( onGround_ )
		{
			speed_ -= airDeccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		else
		{
			speed_ -= deccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		if ( speed_ < 0 )
		{
			speed_ = 0;
		}
	}
	pos_.x += speed_;
	
	if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		JumpStart();
	}

	if ( pos_.y < 600 )
	{
		onGround_ = true;
	}

	if ( onGround_ )
	{
		if (isJump_ )
		{
			Jump();
		}
		else
		{
			Falling();
		}
	}
}

void Player::JumpStart()
{
	onGround_ = true;

	isJump_ = true;

	fallSpeed_ += jumpInitialVelocity_;
}

void Player::Jump()
{
	fallSpeed_ += jumpAcceleration_;

	pos_.y += fallSpeed_;

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE)||fallSpeed_>=0)
	{
		isJump_ = false;
		
		fallSpeed_/=3;
	}

}

void Player::Falling()
{
	fallSpeed_ += gravityAcceleration_;

	pos_.y += fallSpeed_;

	if ( pos_.y >= 600 )
	{
		onGround_ = false;

		fallSpeed_ = 0;

		pos_.y = 600;
	}
}

void Player::Attack()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && attackZ_ != nullptr && attackInterval_ == 0 )
	{
		attackZ_->AttackInit(pos_,direction_);

		speed_ = 0;

		fallSpeed_ = 0;

		attackInterval_ = attackZ_->GetInterval();
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_X) && attackX_ != nullptr && attackInterval_ == 0 )
	{
		attackX_->AttackInit(pos_,direction_);

		speed_ = 0;

		fallSpeed_ = 0;

		attackInterval_ = attackX_->GetInterval();
	}

	if ( attackZ_ != nullptr )
	{
		attackZ_->Attack();
	}

	if ( attackX_ != nullptr )
	{
		attackX_->Attack();
	}

	PlayerBulletManager::Instance()->Update();
}

float Player::IsDamage()
{
	float Damage=0;

	if ( attackX_->GetAttack())
	{
		  Damage=attackX_->GetPow() * changePow_;

		return Damage;
	}
	else
	{
		Damage = attackZ_->GetPow() * changePow_;

		return Damage;
	}
}
void Player::OnCollsionEnemy(int32_t Damage)
{
	hp_ -= Damage * changeDef_;

}
void Player::ChangeAttackZ(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		attackZ_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		attackZ_ = std::make_unique<PlayerAttackWeapon>();
	}
}

void Player::ChangeAttackX(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		attackX_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		attackX_ = std::make_unique<PlayerAttackWeapon>();
	}
}

void Player::AddSpd(int32_t spd)
{
	changeSpd_ += float(spd)/100.0f;//パーセントを実数値に戻す
}

void Player::AddPow(int32_t pow)
{
	changePow_ += float(pow)/100.0f;
}

void Player::AddDef(int32_t def)
{
	changeDef_ += float(def)/100.0f;
}

void Player::AddMaxHp(int32_t maxHp)
{
	changeMaxHp_ += float(maxHp) / 100.0f;
}

void Player::AddCost(int32_t cost)
{
	nowCost += cost;
}

void Player::Draw()
{
	PlayerBulletManager::Instance()->Draw();

	float leftPos = pos_.x - drawSize_.x / 2;
	float rightPos = pos_.x + drawSize_.x / 2;
	float upPos = pos_.y - drawSize_.y / 2;
	float downPos = pos_.y + drawSize_.y / 2;

	DrawBox(leftPos,upPos,rightPos,downPos,GetColor(255,255,255),true);
	DrawBox(pos_.x - colisionSize_.x / 2,pos_.y - colisionSize_.y / 2,pos_.x + colisionSize_.x/2,pos_.y + colisionSize_.y/2,GetColor(255,0,0),false);
	//向いてる方向の視覚化
	if ( direction_ )
	{
		DrawBox(rightPos,upPos,rightPos - 5,upPos + 5,GetColor(255,0,0),true);
	} 
	else
	{
		DrawBox(leftPos,upPos,leftPos + 5,upPos + 5,GetColor(255,0,0),true);
	}

	if ( attackZ_ != nullptr )
	{
		attackZ_->Draw();
	}

	if ( attackX_ != nullptr )
	{
		attackX_->Draw();
	}
}

void Player::Load()
{
	std::ifstream file;

	file.open("Resources/Player/Player.json");

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonObject;

	file >> jsonObject;

	assert(jsonObject.is_object());
	assert(jsonObject.contains("name"));
	assert(jsonObject[ "name" ].is_string());

	std::string lName = jsonObject[ "name" ].get<std::string>();

	assert(lName.compare("Player") == 0);

	topSpeed_= jsonObject[ "TopSpeed" ];
	acceleration_= jsonObject[ "Acceleration" ];
	airAcceleration_ = jsonObject[ "AirAcceleration" ];
	deccelaration_ = jsonObject[ "Deccelaration" ];
	airDeccelaration_ = jsonObject[ "AirDeccelaration" ];
	gravityAcceleration_ = jsonObject[ "GravityAcceleration" ];
	jumpAcceleration_ = jsonObject[ "JumpAcceleration" ];
	jumpInitialVelocity_ = jsonObject[ "JumpInitialVelocity" ];
}
