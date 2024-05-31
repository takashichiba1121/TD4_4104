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

	ChangeAttack("Fist");
}

void Player::Update()
{

	if ( attackInterval_ > 0 )
	{
		attackInterval_--;
	}

	if ( attack_->GetAttack() == false )
	{
		Move();
	}

	Attack();

#ifdef _DEBUG
	if ( Input::Instance()->TriggerKey(KEY_INPUT_2) )
	{
		ChangeAttack("Fist");
	}
	if ( Input::Instance()->TriggerKey(KEY_INPUT_3) )
	{
		ChangeAttack("Weapon");
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_1) )
	{
		Load();
	}


	ImGui::Begin("player");

	ImGui::Text("Jump");

	ImGui::SliderFloat("JumpAcceleration",&jumpAcceleration_,0.0f,-20.0f,"%2.0f");

	ImGui::SliderFloat("GravityAcceleration",&gravityAcceleration_,0.0f,5.0f,"%1.1f");

	ImGui::Text("Move");

	ImGui::SliderFloat("Acceleration",&acceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirAcceleration",&airAcceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("Deccelaration",&deccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirDeccelaration_",&airDeccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("TopSpeed",&topSpeed_,200.0f,0.0f,"%2.0f");

	ImGui::Text("attackInterval %d",attackInterval_);

	ImGui::Text("speed %f",speed_);

	//if ( ImGui::Button("save") )
	//{
	//	
	//}

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		direction_ = false;
		if ( speed_ > -topSpeed_ )
		{
			if ( onGround_ )
			{
				speed_ -= airAcceleration_/60.0f;
			}
			else
			{
				speed_ -= acceleration_/60.0f;
			}
		}
		else
		{
			speed_ = -topSpeed_;
		}
	}
	else if ( speed_ < 0 )
	{
		if ( onGround_ )
		{
			speed_ += airDeccelaration_ / 60.0f;
		}
		else
		{
			speed_ += deccelaration_ / 60.0f;
		}
		if ( speed_ > 0 )
		{
			speed_ = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		direction_ = true;
		if ( speed_ < topSpeed_ )
		{
			if ( onGround_ )
			{
				speed_ += airAcceleration_/60.0f;
			}
			else
			{
				speed_ += acceleration_/60.0f;
			}
		}
		else
		{
			speed_ = topSpeed_;
		}
	}
	else if ( speed_ > 0 )
	{
		if ( onGround_ )
		{
			speed_ -= airDeccelaration_ / 60.0f;
		}
		else
		{
			speed_ -= deccelaration_ / 60.0f;
		}
		if ( speed_ < 0 )
		{
			speed_ = 0;
		}
	}
	pos_.x += speed_;

	if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		fallSpeed_ = jumpAcceleration_;

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
		
		fallSpeed_/=5;
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
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && attack_ != nullptr && attackInterval_ == 0 )
	{
		attack_->AttackInit(pos_,direction_);

		speed_ = 0;

		fallSpeed_ = 0;

		attackInterval_ = attack_->GetInterval();
	}

	if ( attack_ != nullptr )
	{
		attack_->Attack();
	}

	PlayerBulletManager::Instance()->Update();
}

void Player::ChangeAttack(std::string attackName)
{
	if (attackName=="Fist" )
	{
		attack_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		attack_ = std::make_unique<PlayerAttackWeapon>();
	}
}

void Player::Draw()
{
	PlayerBulletManager::Instance()->Draw();

	float leftPos = pos_.x - size_.x / 2;
	float rightPos = pos_.x + size_.x / 2;
	float upPos = pos_.y - object.r_.y / 2;
	float downPos = pos_.y + object.r_.y / 2;

	DrawBox(leftPos,upPos,rightPos,downPos,GetColor(255,255,255),true);
	DrawBox(leftPos + colisionSift_.x,upPos + colisionSift_.y,rightPos - colisionSize_.x,downPos - colisionobject.r_.y,GetColor(255,0,0),false);
	if ( direction_ )
	{
		DrawBox(rightPos,upPos,rightPos - 5,upPos + 5,GetColor(255,0,0),true);
	} 
	else
	{
		DrawBox(leftPos,upPos,leftPos + 5,upPos + 5,GetColor(255,0,0),true);
	}

	if ( attack_ != nullptr )
	{
		attack_->Draw();
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
