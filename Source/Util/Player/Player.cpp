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
		if (accel>=-cTopSpeed )		{
			accel -= cSpeed_;
		}
	}
	else if(accel<0)
	{
			accel += cBrake;
		if ( accel > 0 )
		{
			accel = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		if ( accel <= cTopSpeed )
		{
			accel += cSpeed_;
		}
	}
	else if ( accel > 0 )
	{
		accel -= cBrake;
		if ( accel < 0 )
		{
			accel = 0;
		}
	}
	posX_ += accel;

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_)
	{
		fallSpeed_ = StartJumpSpeed_;

		onGround_ = true;
	}

	if ( posY_ < 600 )
	{
		onGround_ = true;
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_Z)&&attack!=nullptr )
	{
		attack->AttackInit();
	}

	if ( onGround_ )
	{
		Jump();
	}

	if ( attack != nullptr )
	{
		attack->Attack();
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
	DrawBox(posX_,posY_,posX_ + 20,posY_ + 60,GetColor(255,255,255),true);

	if ( attack != nullptr )
	{
		attack->Draw();
	}
}
