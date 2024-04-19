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
		posX_ -=cSpeed_;
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		posX_ += cSpeed_;
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_)
	{
		fallSpeed_ = StartJumpSpeed_;

		onGround_ = true;
	}

	if ( posY_ < 600 )
	{
		onGround_ = true;
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_Z) )
	{
		isAttack_ = true;
	}

	if ( onGround_ )
	{
		Jump();
	}

	if ( isAttack_ )
	{
		Attack();
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

void Player::Attack()
{

}

void Player::Draw()
{
	DrawBox(posX_,posY_,posX_ + 20,posY_ + 60,GetColor(255,255,255),true);
}
