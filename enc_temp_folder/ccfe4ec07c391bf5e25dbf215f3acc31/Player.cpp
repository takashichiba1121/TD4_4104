#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"

void Player::Initialze()
{
	posX_ = GameConfig::GetGameConfig()->windowWidth/2;

	posY_ = GameConfig::GetGameConfig()->windowHeight / 2;
}

void Player::Update()
{
	if (Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		if (speed_>=-cTopSpeed_ ){
			if (onGround_ )
			{
				speed_ -= cAirAcceleration_;
			}
			else
			{
				speed_ -= cAcceleration_;
			}
		}
	}
	else if(speed_<0)
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
			speed_ += cAcceleration_;
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
		if (speed_ < 0 )
		{
			speed_ = 0;
		}
	}
	posX_ += speed_;

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_)
	{
		fallSpeed_ = StartJumpSpeed_;

		onGround_ = true;
	}

	if ( posY_ < 600 )
	{
		onGround_ = true;
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_Z)&&attack_!=nullptr )
	{
		attack_->AttackInit();
	}

	if ( onGround_ )
	{
		Jump();
	}

	if ( attack_ != nullptr )
	{
		attack_->Attack();
	}

#ifdef _DEBUG
	ImGui::Begin("player");

	ImGui::Text("isJump%d",onGround_);

	ImGui::Text("isAttack%d",isAttack_);

	ImGui::End();

#endif
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
	DrawBox(posX_,posY_,posX_ + 35,posY_ + 61,GetColor(255,255,255),true);
	DrawBox(posX_ + 5,posY_ + 5,posX_ + 30,posY_ + 56,GetColor(255,0,0),false);

	if ( attack_ != nullptr )
	{
		attack_->Draw();
	}
}
