#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"
#include"PlayerAttackFist.h"

void Player::Initialze()
{
	posX_ = GameConfig::GetGameConfig()->windowWidth/2;

	posY_ = GameConfig::GetGameConfig()->windowHeight / 2;
	
	attack_ = std::make_unique<PlayerAttackFist>();
}

void Player::Update()
{
	if (attack_->GetAttack()==false )
	{
		Move();
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_Z)&&attack_!=nullptr )
	{
		attack_->AttackInit();

		speed_ = 0;

		fallSpeed_ = 0;
	}

	if ( attack_ != nullptr )
	{
		attack_->Attack();
	}

#ifdef _DEBUG
	ImGui::Begin("player");

	ImGui::SliderFloat("gravityAcceleration",&gravityAcceleration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("Acceleration",&acceleration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("AirAcceleration",&cAirAcceleration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("Deccelaration",&cDeccelaration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("AirDeccelaration_",&cAirDeccelaration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("TopSpeed",&cTopSpeed_,100.0f,0.0f,"%3.1f");

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		if ( speed_ >= -cTopSpeed_ )
		{
			if ( onGround_ )
			{
				speed_ -= cAirAcceleration_;
			}
			else
			{
				speed_ -= acceleration_;
			}
		}
	}
	else if ( speed_ < 0 )
	{
		if ( onGround_ )
		{
			speed_ += cAirDeccelaration_;
		}
		else
		{
			speed_ += cDeccelaration_;
		}
		if ( speed_ > 0 )
		{
			speed_ = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		if ( onGround_ )
		{
			speed_ += cAirAcceleration_;
		}
		else
		{
			speed_ += acceleration_;
		}
	}
	else if ( speed_ > 0 )
	{
		if ( onGround_ )
		{
			speed_ -= cAirDeccelaration_;
		}
		else
		{
			speed_ -= cDeccelaration_;
		}
		if ( speed_ < 0 )
		{
			speed_ = 0;
		}
	}
	posX_ += speed_;

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		fallSpeed_ = StartJumpSpeed_;

		onGround_ = true;
	}

	if ( posY_ < 600 )
	{
		onGround_ = true;
	}

	if ( onGround_ )
	{
		Jump();
	}
}

void Player::Jump()
{
	fallSpeed_ += gravityAcceleration_;

	posY_ += fallSpeed_;

	if (posY_>=600 )
	{
		onGround_ = false;

		fallSpeed_ = 0;

		posY_ = 600;
	}
}

void Player::Draw()
{
	DrawBox(posX_,posY_,posX_ + 16,posY_ + 32,GetColor(255,255,255),true);

	if ( attack_ != nullptr )
	{
		attack_->Draw();
	}
}
