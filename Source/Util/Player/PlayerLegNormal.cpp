#include "PlayerLegNormal.h"
#include"Input.h"
#include"DxlibInclude.h"
#include"json.hpp"
#include <fstream>
#include"GameConfig.h"

void PlayerLegNormal::Initialize(Vector2* playerVelocity,bool* direction,float* changeAcl)
{
	playerVelocity_ = playerVelocity;

	direction_ = direction;

	changeAcl_ = changeAcl;
}

void PlayerLegNormal::Move(bool DirBOTTOM)
{
	if ( Input::Instance()->PushKey(KEY_INPUT_LEFT) || Input::Instance()->PushKey(KEY_INPUT_A) && isEvasionRoll_ == false )
	{
		*direction_ = false;
		if ( playerVelocity_->x > topSpeed_ * *changeAcl_ )
		{
			if ( onGround_ )
			{
				playerVelocity_->x -= ( airAcceleration_ * *changeAcl_ ) / GameConfig::GetGameConfig()->fps;
			}
			else
			{
				playerVelocity_->x -= ( acceleration_ * *changeAcl_ ) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			playerVelocity_->x = -topSpeed_ * *changeAcl_;
		}
	}
	else if ( playerVelocity_->x < 0 )
	{
		if ( onGround_ )
		{
			playerVelocity_->x += airDeccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		else
		{
			playerVelocity_->x += deccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		if ( playerVelocity_->x > 0 )
		{
			playerVelocity_->x = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_RIGHT) || Input::Instance()->PushKey(KEY_INPUT_D) && isEvasionRoll_ == false )
	{
		*direction_ = true;
		if ( playerVelocity_->x < topSpeed_ * *changeAcl_ )
		{
			if ( onGround_ )
			{
				playerVelocity_->x += ( airAcceleration_ * *changeAcl_ ) / GameConfig::GetGameConfig()->fps;
			}
			else
			{
				playerVelocity_->x += ( acceleration_ * *changeAcl_ ) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			playerVelocity_->x = topSpeed_ * *changeAcl_;
		}
	}
	else if ( playerVelocity_->x > 0 )
	{
		if ( onGround_ )
		{
			playerVelocity_->x -= airDeccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		else
		{
			playerVelocity_->x -= deccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		if ( playerVelocity_->x < 0 )
		{
			playerVelocity_->x = 0;
		}
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		JumpStart();
	}


	if (DirBOTTOM )
	{
		onGround_ = true;
	}

	if ( onGround_ )
	{
		if ( isJump_ )
		{
			Jump();
		}
		else
		{
			Falling();
		}
	}
}

void PlayerLegNormal::JumpStart()
{

}

void PlayerLegNormal::Jump()
{

}

void PlayerLegNormal::EvasionRoll()
{

}

void PlayerLegNormal::Falling()
{

}

void PlayerLegNormal::Draw()
{

}

void PlayerLegNormal::Load()
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

	topSpeed_ = jsonObject[ "TopSpeed" ];
	acceleration_ = jsonObject[ "Acceleration" ];
	airAcceleration_ = jsonObject[ "AirAcceleration" ];
	deccelaration_ = jsonObject[ "Deccelaration" ];
	airDeccelaration_ = jsonObject[ "AirDeccelaration" ];
	gravityAcceleration_ = jsonObject[ "GravityAcceleration" ];
	jumpAcceleration_ = jsonObject[ "JumpAcceleration" ];
	jumpInitialVelocity_ = jsonObject[ "JumpInitialVelocity" ];
}
