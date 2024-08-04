#include"PLayerLegFenrir.h"
#include"Input.h"
#include"DxlibInclude.h"
#include"json.hpp"
#include <fstream>
#include"GameConfig.h"
#include"PlayerBulletManager.h"

void PlayerLegFenrir::Initialize(Vector2* playerVelocity,bool* direction,float* changeAcl)
{
	playerVelocity_ = playerVelocity;

	direction_ = direction;

	changeAcl_ = changeAcl;

	Load();

	PlayerStandTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\fenrirStand.png"));

	PlayerJumpTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\fenrirJumpUp.png"));

	PlayerDownTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\fenrirJumpDown.png"));

	PlayerDushTexture_ = LoadGraph(std::string("Resources\\Player\\Parts\\fenrirDush.png"));
}

void PlayerLegFenrir::Move(bool DirBOTTOM,bool isAttack,const Vector2& pos,const float pow)
{
	oldIsDirBottom_ = isDirBottom_;

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
	if ( ( Input::Instance()->PushKey(KEY_INPUT_RIGHT) || Input::Instance()->PushKey(KEY_INPUT_D) ) && !isEvasionRoll_ && !isAttack && !isJump_ )
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
			newBullet->Initialize({ 0,0 },pos,60,pow,1,1,PlayerBullet::Type::ICED);
			PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));

			bulletInterval_ = 0;
		}
	}
}

void PlayerLegFenrir::JumpStart()
{
	onGround_ = true;

	isJump_ = true;

	playerVelocity_->y += jumpInitialVelocity_;

	PlayerJumpTextureCount_ = 0;

	isBullet = true;

	if ( *direction_ )
	{
		playerVelocity_->x += evasionRollSpeed_;
	}
	else
	{
		playerVelocity_->x += -evasionRollSpeed_;
	}
}

void PlayerLegFenrir::Jump()
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

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE) || playerVelocity_->y >= 0 )
	{
		isJump_ = false;

		playerVelocity_->y /= 3;
	}

}

void PlayerLegFenrir::EvasionRoll()
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

void PlayerLegFenrir::Falling()
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

		isBullet = false;

	}
}

void PlayerLegFenrir::Draw(const Vector2& pos,const Vector2& size,Vector2 scroll)
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

void PlayerLegFenrir::Load()
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
