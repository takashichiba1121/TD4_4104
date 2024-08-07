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

	PlayerStandTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\cerberusStand.png"));

	PlayerJumpTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\cerberusJumpUp.png"));

	PlayerDownTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\cerberusJumpDown.png"));

	PlayerDushTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\cerberusDush.png"));

	evasionRollSoundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_DodgeRoll.mp3"));

	cerberusJumpSoundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_leg_cerberus_Jump.mp3"));

	landingSoundId_ = LoadSoundMem(std::string("Resources\\Sound\\Player\\SFX_player_Landing.mp3"));
}

void PlayerLegCerberus::Move(bool DirBOTTOM,bool isAttack,const Vector2& pos,const float pow)
{
	oldIsDirBottom_ = isDirBottom_;

	isDirBottom_ = DirBOTTOM;

	int X = Input::Instance()->PadX();

	if ( ( Input::Instance()->PushKey(KEY_INPUT_LEFT) || Input::Instance()->PushKey(KEY_INPUT_A) ) || X <= -500 && !isEvasionRoll_ && !isAttack && !isJump_ )
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
	if ( ( Input::Instance()->PushKey(KEY_INPUT_RIGHT) || Input::Instance()->PushKey(KEY_INPUT_D) )|| X >= 500 && !isEvasionRoll_ && !isAttack&& !isJump_ )
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

	if ( Input::Instance()->PushKey(KEY_INPUT_SPACE) || Input::Instance()->PushPadKey(PAD_INPUT_1) && !onGround_ )
	{
		JumpStart();
	}

	EvasionRoll();

	if ( !isDirBottom_ )
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
	PlaySoundMem(cerberusJumpSoundId_,DX_PLAYTYPE_BACK);
}

void PlayerLegCerberus::Jump()
{
	playerVelocity_->y += jumpAcceleration_;

	if ( !isDirBottom_ && !oldIsDirBottom_ )
	{
		PlayerDownTextureCount_++;
		if ( PlayerDownTextureCount_ == 40 )
		{
			PlayerDownTextureCount_ = 0;
		}
	}

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE) || Input::Instance()->ReleasePadKey(PAD_INPUT_1) || playerVelocity_->y >= 0 )
	{
		isJump_ = false;

		playerVelocity_->y /= 3;

		isBullet = false;
	}

}

void PlayerLegCerberus::EvasionRoll()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Q) || Input::Instance()->TriggerPadKey(PAD_INPUT_2) && isEvasionRoll_ == false )
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
		PlaySoundMem(evasionRollSoundId_,DX_PLAYTYPE_BACK);
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

		//PlaySoundMem(landingSoundId_,DX_PLAYTYPE_BACK);
	}
}

void PlayerLegCerberus::Draw(const Vector2& pos,const Vector2& size,Vector2 scroll)
{
	float leftPos = scroll.x + pos.x - size.x / 2;
	float rightPos = scroll.x + pos.x + size.x / 2;
	float upPos = scroll.y + pos.y - size.y / 2;
	float downPos = scroll.y + pos.y + size.y / 2;

	if ( isEvasionRoll_ )
	{
		if ( *direction_ )
		{
			DrawRectGraph(leftPos,upPos,PlayerDushTextureCount_ * size.x,0,
				size.x,size.y,PlayerDushTexture_,true,false);
		}
		else
		{
			DrawRectGraph(leftPos,upPos,PlayerDushTextureCount_ * size.x,0,
				size.x,size.y,PlayerDushTexture_,true,true);
		}
	}
	else if ( isJump_ )
	{
		if ( *direction_ )
		{
			DrawRectGraph(leftPos,upPos,( PlayerJumpTextureCount_ / 10 ) * size.x,0,
				size.x,size.y,PlayerJumpTexture_,true,false);
		}
		else
		{
			DrawRectGraph(leftPos,upPos,( PlayerJumpTextureCount_ / 10 ) * size.x,0,
				size.x,size.y,PlayerJumpTexture_,true,true);
		}
	}
	else if ( !isDirBottom_ && !oldIsDirBottom_ )
	{
		if ( *direction_ )
		{
			DrawRectGraph(leftPos,upPos,( PlayerDownTextureCount_ / 10 ) * size.x,0,
				size.x,size.y,PlayerDownTexture_,true,false);
		}
		else
		{
			DrawRectGraph(leftPos,upPos,( PlayerDownTextureCount_ / 10 ) * size.x,0,
				size.x,size.y,PlayerDownTexture_,true,true);
		}
	}
	else if ( isWalk )
	{
		if ( *direction_ )
		{
			DrawRectGraph(leftPos,upPos,( PlayerStandTextureCount_ / 2 ) * size.x,0,
				size.x,size.y,PlayerStandTexture_,true,false);
		}
		else
		{
			DrawRectGraph(leftPos,upPos,( PlayerStandTextureCount_ / 2 ) * size.x,0,
				size.x,size.y,PlayerStandTexture_,true,true);
		}
	}
	else
	{
		if ( *direction_ )
		{
			DrawRectGraph(leftPos,upPos,( PlayerStandTextureCount_ / 2 ) * size.x,0,
				size.x,size.y,PlayerStandTexture_,true,false);
		}
		else
		{
			DrawRectGraph(leftPos,upPos,( PlayerStandTextureCount_ / 2 ) * size.x,0,
				size.x,size.y,PlayerStandTexture_,true,true);
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
