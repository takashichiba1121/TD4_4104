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

	Load();

	LoadDivGraph("Resources/Player/PlayerStand_sheet.png",10,10,1,128,128,( int* ) PlayerStandTexture_);

	LoadDivGraph("Resources/Player/PlayerJumpUp.png",4,4,1,128,128,( int* ) PlayerJumpTexture_);
}

void PlayerLegNormal::Move(bool DirBOTTOM,bool isAttack)
{
	isDirBottom_ = DirBOTTOM;

	if ( ( Input::Instance()->PushKey(KEY_INPUT_LEFT) || Input::Instance()->PushKey(KEY_INPUT_A) ) && !isEvasionRoll_ && !isAttack )
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

		isWalk = true;
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
		isWalk = false;

		PlayerStandTextureCount_++;
		if ( PlayerStandTextureCount_ == 20 )
		{
			PlayerStandTextureCount_ = 0;
		}
	}
	if ( ( Input::Instance()->PushKey(KEY_INPUT_RIGHT) || Input::Instance()->PushKey(KEY_INPUT_D) ) && !isEvasionRoll_ && !isAttack )
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

		isWalk = true;
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

		isWalk = false;

		PlayerStandTextureCount_++;
		if ( PlayerStandTextureCount_ == 20 )
		{
			PlayerStandTextureCount_ = 0;
		}
	}

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		JumpStart();
	}

	EvasionRoll();

	if ( !DirBOTTOM )
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
	onGround_ = true;

	isJump_ = true;

	playerVelocity_->y += jumpInitialVelocity_;

	PlayerJumpTextureCount_ = 0;
}

void PlayerLegNormal::Jump()
{
	playerVelocity_->y += jumpAcceleration_;

	PlayerJumpTextureCount_++;
	if ( PlayerJumpTextureCount_ == 4 )
	{
		PlayerJumpTextureCount_ = 0;
	}

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE) || playerVelocity_->y >= 0 )
	{
		isJump_ = false;

		playerVelocity_->y /= 3;
	}

}

void PlayerLegNormal::EvasionRoll()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Q) && isEvasionRoll_ == false )
	{
		isEvasionRoll_ = true;

		if ( *direction_ )
		{
			*playerVelocity_ = { evasionRollSpeed_,0 };
		}
		else
		{
			*playerVelocity_ = { -evasionRollSpeed_,0 };
		}
	}
	if ( isEvasionRoll_ )
	{
		if ( *direction_ )
		{
			if ( playerVelocity_->x < 6 )
			{
				isEvasionRoll_ = false;
			}
		}
		else
		{
			if ( playerVelocity_->x > -6 )
			{
				isEvasionRoll_ = false;
			}
		}
	}
}

void PlayerLegNormal::Falling()
{
	playerVelocity_->y += gravityAcceleration_;

	if ( isDirBottom_ )
	{
		onGround_ = false;

		playerVelocity_->y = 0;

	}
}

void PlayerLegNormal::Draw(Vector2 pos,Vector2 size)
{
	float leftPos = pos.x - size.x / 2;
	float rightPos = pos.x + size.x / 2;
	float upPos = pos.y - size.y / 2;
	float downPos = pos.y + size.y / 2;

	if (isEvasionRoll_ )
	{
		if ( *direction_ )
		{
			DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
		else
		{
			DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
	}
	else if (onGround_ )
	{
		if ( isJump_ )
		{
			if ( *direction_ )
			{
				DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerJumpTexture_[ PlayerJumpTextureCount_ ],TRUE);
			}
			else
			{
				DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerJumpTexture_[ PlayerJumpTextureCount_ ],TRUE);
			}
		}
		else
		{
			if ( *direction_ )
			{
				DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
			}
			else
			{
				DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
			}
		}
	}
	else if(isWalk)
	{
		if ( *direction_ )
		{
			DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
		else
		{
			DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
	}
	else
	{
		if ( *direction_ )
		{
			DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
		else
		{
			DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerStandTexture_[ PlayerStandTextureCount_ / 2 ],TRUE);
		}
	}
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
