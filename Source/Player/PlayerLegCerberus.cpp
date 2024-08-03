#include"PLayerLegCerberus.h"
#include"Input.h"
#include"DxlibInclude.h"
#include"json.hpp"
#include <fstream>
#include"GameConfig.h"
#include"PlayerBulletManager.h"

void PlayerLegCerberus::Initialize(Vector2* playerVelocity,bool* direction,float* changeAcl)
{
	playerVelocity_ = playerVelocity;

	direction_ = direction;

	changeAcl_ = changeAcl;

	Load();

	LoadDivGraph("Resources/Player/PlayerStand_sheet.png",10,10,1,128,128,( int* ) PlayerStandTexture_);

	LoadDivGraph("Resources/Player/PlayerJumpUp.png",4,4,1,128,128,( int* ) PlayerJumpTexture_);

	LoadDivGraph("Resources/Player/PlayerJumpDown.png",4,4,1,128,128,( int* ) PlayerDownTexture_);

	LoadDivGraph("Resources/Player/PlayerDush.png",5,5,1,128,128,( int* ) PlayerDushTexture_);
}

void PlayerLegCerberus::Move(bool DirBOTTOM,bool isAttack,const Vector2& pos,const float pow)
{
	isDirBottom_ = DirBOTTOM;

	if ( ( Input::Instance()->PushKey(KEY_INPUT_LEFT) || Input::Instance()->PushKey(KEY_INPUT_A) ) && !isEvasionRoll_ && !isAttack && !isJump_ )
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
	if ( ( Input::Instance()->PushKey(KEY_INPUT_RIGHT) || Input::Instance()->PushKey(KEY_INPUT_D) ) && !isEvasionRoll_ && !isAttack&& !isJump_ )
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

	if ( isBullet )
	{
		bulletInterval_++;
		if ( bulletInterval_ == MAX_BULLET_INTERVAL_ )
		{
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize({ 0,0 },pos,60,pow,1,1,PlayerBullet::Type::BURN);
			PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));

			bulletInterval_ = 0;
		}
	}
}

void PlayerLegCerberus::JumpStart()
{
	onGround_ = true;

	isJump_ = true;

	playerVelocity_->y += jumpInitialVelocity_;

	PlayerJumpTextureCount_ = 0;

	isBullet = true;

	if ( *direction_ )
	{
		playerVelocity_->x += evasionRollSpeed_ * 1.5f;
	}
	else
	{
		playerVelocity_->x += -evasionRollSpeed_ * 1.5f;
	}
}

void PlayerLegCerberus::Jump()
{
	playerVelocity_->y += jumpAcceleration_;

	PlayerJumpTextureCount_++;
	if ( PlayerJumpTextureCount_ == 40 )
	{
		PlayerJumpTextureCount_ = 0;
	}

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE) || playerVelocity_->y >= 0 )
	{
		isJump_ = false;

		playerVelocity_->y /= 3;

		isBullet = false;
	}

}

void PlayerLegCerberus::EvasionRoll()
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
		PlayerDushTextureCount_++;
		if ( PlayerDushTextureCount_ == 4 )
		{
			PlayerDushTextureCount_ = 0;
		}

		if ( *direction_ )
		{
			if ( playerVelocity_->x < 6 )
			{
				isEvasionRoll_ = false;
			}
			PlayerDushTextureCount_ = 0;
		}
		else
		{
			if ( playerVelocity_->x > -6 )
			{
				isEvasionRoll_ = false;
			}
			PlayerDushTextureCount_ = 0;
		}
	}
}

void PlayerLegCerberus::Falling()
{
	playerVelocity_->y += gravityAcceleration_;

	PlayerDownTextureCount_++;
	if ( PlayerDownTextureCount_ == 40 )
	{
		PlayerDownTextureCount_ = 0;
	}

	if ( isDirBottom_ )
	{
		onGround_ = false;

		playerVelocity_->y = 0;

		PlayerDownTextureCount_ = 0;

	}
}

void PlayerLegCerberus::Draw(const Vector2& pos,const Vector2& size)
{
	float leftPos = pos.x - size.x / 2;
	float rightPos = pos.x + size.x / 2;
	float upPos = pos.y - size.y / 2;
	float downPos = pos.y + size.y / 2;

	if ( isEvasionRoll_ )
	{
		if ( *direction_ )
		{
			DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerDushTexture_[ PlayerDushTextureCount_ ],TRUE);
		}
		else
		{
			DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerDushTexture_[ PlayerDushTextureCount_ ],TRUE);
		}
	}
	else if ( onGround_ )
	{
		if ( isJump_ )
		{
			if ( *direction_ )
			{
				DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerJumpTexture_[ PlayerJumpTextureCount_ / 10 ],TRUE);
			}
			else
			{
				DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerJumpTexture_[ PlayerJumpTextureCount_ / 10 ],TRUE);
			}
		}
		else
		{
			if ( *direction_ )
			{
				DrawExtendGraph(leftPos,upPos,rightPos,downPos,PlayerDownTexture_[ PlayerDownTextureCount_ / 10 ],TRUE);
			}
			else
			{
				DrawExtendGraph(rightPos,upPos,leftPos,downPos,PlayerDownTexture_[ PlayerDownTextureCount_ / 10 ],TRUE);
			}
		}
	}
	else if ( isWalk )
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

void PlayerLegCerberus::Load()
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

	topSpeed_ = jsonObject[ "TopSpeed" ] * 1.5f;
	acceleration_ = jsonObject[ "Acceleration" ] * 1.5f;
	airAcceleration_ = jsonObject[ "AirAcceleration" ] * 1.5f;
	deccelaration_ = jsonObject[ "Deccelaration" ] * 1.5f;
	airDeccelaration_ = jsonObject[ "AirDeccelaration" ] * 1.5f;
	gravityAcceleration_ = jsonObject[ "GravityAcceleration" ];
	jumpAcceleration_ = jsonObject[ "JumpAcceleration" ];
	jumpInitialVelocity_ = jsonObject[ "JumpInitialVelocity" ];
}
