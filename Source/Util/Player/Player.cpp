#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"
#include"PlayerAttackFist.h"

void Player::Initialze()
{
	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	attack_ = std::make_unique<PlayerAttackFist>();
}

void Player::Update()
{
	if ( attack_->GetAttack() == false )
	{
		Move();
	}
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && attack_ != nullptr )
	{
		attack_->AttackInit(pos_,direction_);

		speed_ = 0;

		fallSpeed_ = 0;
	}

	if ( attack_ != nullptr )
	{
		attack_->Attack();
	}

#ifdef _DEBUG
	ImGui::Begin("player");

	ImGui::Text("Jump");

	ImGui::SliderFloat("JumpAcceleration",&jumpAcceleration_,0.0f,-20.0f,"%2.0f");

	ImGui::SliderFloat("GravityAcceleration",&gravityAcceleration_,0.0f,1.0f,"%1.2f");

	ImGui::Text("Move");

	ImGui::SliderFloat("Acceleration",&acceleration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("AirAcceleration",&airAcceleration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("Deccelaration",&deccelaration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("AirDeccelaration_",&airDeccelaration_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat("TopSpeed",&topSpeed_,200.0f,0.0f,"%2.0f");

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		direction_ = false;
		if ( speed_ >= -topSpeed_ )
		{
			if ( onGround_ )
			{
				speed_ -= airAcceleration_;
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
			speed_ += airDeccelaration_;
		}
		else
		{
			speed_ += deccelaration_;
		}
		if ( speed_ > 0 )
		{
			speed_ = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		direction_ = true;
		if ( onGround_ )
		{
			speed_ += airAcceleration_;
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
			speed_ -= airDeccelaration_;
		}
		else
		{
			speed_ -= deccelaration_;
		}
		if ( speed_ < 0 )
		{
			speed_ = 0;
		}
	}
	pos_.x += speed_;

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		fallSpeed_ = jumpAcceleration_;

		onGround_ = true;
	}

	if ( pos_.y < 600 )
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

	pos_.y += fallSpeed_;

	if ( pos_.y >= 600 )
	{
		onGround_ = false;

		fallSpeed_ = 0;

		pos_.y = 600;
	}
}

void Player::Draw()
{
	float leftPos = pos_.x - size_.x / 2;
	float rightPos = pos_.x + size_.x / 2;
	float upPos = pos_.y - size_.y / 2;
	float downPos = pos_.y + size_.y / 2;

	DrawBox(leftPos,upPos,rightPos,downPos,GetColor(255,255,255),true);
	DrawBox(leftPos + colisionSift_.x,upPos + colisionSift_.y,rightPos - colisionSize_.x,downPos - colisionSize_.y,GetColor(255,0,0),false);
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