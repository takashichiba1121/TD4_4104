#include "Player.h"
#include"DxlibInclude.h"
#include"GameConfig.h"
#include"Input.h"
#include"imgui.h"
#include"PlayerAttackFist.h"
#include"PlayerAttackWeapon.h"
#include"PlayerBulletManager.h"
#include"json.hpp"
#include <fstream>

#include"CollisionManager.h"

void Player::Initialize()
{
	hitboxSize_ = { 28,54 };

	drawSize_ = { 34,60 };

	pos_.x = GameConfig::GetGameConfig()->windowWidth / 2;

	pos_.y = GameConfig::GetGameConfig()->windowHeight / 2;

	Load();

	ChangeAttackZ("Fist");

	ChangeAttackX("Weapon");

	hp_ = maxHp_;

	islive_ = true;

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });

	shape_ = new CircleShape();
	shape_->SetRadius(drawSize_.y / 2);

	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYRE);
	SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYRE);

	CollisionManager::GetInstance()->AddObject(this);
}

void Player::Update()
{

	if ( attackInterval_ > 0 )
	{
		attackInterval_--;
	}

	if ( attackZ_->GetAttack() == false&& attackX_->GetAttack() == false )
	{
		Move();
	}

	Attack();

#ifdef _DEBUG

	ImGui::Begin("PlayerStatus");

	if ( ImGui::Button("Load") )
	{
		Load();
	}

	ImGui::Text("Jump");

	ImGui::SliderFloat("JumpInitialVelocity",&jumpInitialVelocity_,0.0f,-20.0f,"%2.0f");

	ImGui::SliderFloat("JumpAcceleration",&jumpAcceleration_,0.0f,20.0f,"%2.1f");

	ImGui::SliderFloat("GravityAcceleration",&gravityAcceleration_,0.0f,5.0f,"%1.1f");

	ImGui::Text("Move");

	ImGui::SliderFloat("Acceleration",&acceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirAcceleration",&airAcceleration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("Deccelaration",&deccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("AirDeccelaration_",&airDeccelaration_,0.0f,100.0f,"%3.0f");

	ImGui::SliderFloat("TopSpeed",&topSpeed_,0.0f,200.0f,"%3.0f");

	ImGui::End();

	ImGui::Begin("PlayerSituation");

	ImGui::Text("vec:%f,%f",velocity_.x,velocity_.y);

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->PushKey(KEY_INPUT_A) )
	{
		direction_ = false;
		if ( velocity_.x > -topSpeed_*changeSpd_)
		{
			if ( onGround_ )
			{
				velocity_.x -= ( airAcceleration_ * changeSpd_ ) /GameConfig::GetGameConfig()->fps;
			}
			else
			{
				velocity_.x -= (acceleration_*changeSpd_) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			velocity_.x = -topSpeed_ * changeSpd_;
		}
	}
	else if ( velocity_.x < 0 )
	{
		if ( onGround_ )
		{
			velocity_.x += airDeccelaration_ /GameConfig::GetGameConfig()->fps;
		}
		else
		{
			velocity_.x += deccelaration_ /GameConfig::GetGameConfig()->fps;
		}
		if ( velocity_.x > 0 )
		{
			velocity_.x = 0;
		}
	}
	if ( Input::Instance()->PushKey(KEY_INPUT_D) )
	{
		direction_ = true;
		if ( velocity_.x < topSpeed_ *changeSpd_ )
		{
			if ( onGround_ )
			{
				velocity_.x += ( airAcceleration_ * changeSpd_ ) / GameConfig::GetGameConfig()->fps;
			}
			else
			{
				velocity_.x += ( acceleration_ * changeSpd_ ) / GameConfig::GetGameConfig()->fps;
			}
		}
		else
		{
			velocity_.x = topSpeed_ * changeSpd_;
		}
	}
	else if ( velocity_.x > 0 )
	{
		if ( onGround_ )
		{
			velocity_.x -= airDeccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		else
		{
			velocity_.x -= deccelaration_ / GameConfig::GetGameConfig()->fps;
		}
		if ( velocity_.x < 0 )
		{
			velocity_.x = 0;
		}
	}
	
	if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) && !onGround_ )
	{
		JumpStart();
	}


	if ( !(GetOnDir() & 0b1 << OnDir::BOTTOM) )
	{
		onGround_ = true;
	}

	if ( onGround_ )
	{
		if (isJump_ )
		{
			Jump();
		}
		else
		{
			Falling();
		}
	}

	SetMapChipSpeed(velocity_);

	shape_->SetCenter(pos_);
}

void Player::JumpStart()
{
	onGround_ = true;

	isJump_ = true;

	velocity_.y += jumpInitialVelocity_;
}

void Player::Jump()
{
	velocity_.y += jumpAcceleration_;

	if ( Input::Instance()->ReleaseKey(KEY_INPUT_SPACE)|| velocity_.y >=0)
	{
		isJump_ = false;
		
		velocity_.y /=3;
	}

}

void Player::Falling()
{
	velocity_.y += gravityAcceleration_;

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		onGround_ = false;

		velocity_.y = 0;

	}
}

void Player::Attack()
{
	if ( Input::Instance()->TriggerKey(KEY_INPUT_Z) && attackZ_ != nullptr && attackInterval_ == 0 )
	{
		attackZ_->AttackInit(pos_,direction_);

		velocity_ = { 0,0 };

		attackInterval_ = attackZ_->GetInterval();
	}

	if ( Input::Instance()->TriggerKey(KEY_INPUT_X) && attackX_ != nullptr && attackInterval_ == 0 )
	{
		attackX_->AttackInit(pos_,direction_);

		velocity_ = { 0,0 };

		attackInterval_ = attackX_->GetInterval();
	}

	if ( attackZ_ != nullptr )
	{
		attackZ_->Attack();
	}

	if ( attackX_ != nullptr )
	{
		attackX_->Attack();
	}

	PlayerBulletManager::Instance()->Update();
}

float Player::IsDamage()
{
	float Damage=0;

	if ( attackX_->GetAttack())
	{
		  Damage=attackX_->GetPow() * changePow_;

		return Damage;
	}
	else
	{
		Damage = attackZ_->GetPow() * changePow_;

		return Damage;
	}
}
void Player::Damage(int32_t Damage)
{
	hp_ -= Damage * changeDef_;

}
void Player::ChangeAttackZ(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		attackZ_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		attackZ_ = std::make_unique<PlayerAttackWeapon>();
	}
}

void Player::ChangeAttackX(std::string attackName)
{
	if ( attackName == "Fist" )
	{
		attackX_ = std::make_unique<PlayerAttackFist>();
	}
	if ( attackName == "Weapon" )
	{
		attackX_ = std::make_unique<PlayerAttackWeapon>();
	}
}

void Player::AddSpd(int32_t spd)
{
	changeSpd_ += float(spd)/100.0f;//パーセントを実数値に戻す
}

void Player::AddPow(int32_t pow)
{
	changePow_ += float(pow)/100.0f;
}

void Player::AddDef(int32_t def)
{
	changeDef_ += float(def)/100.0f;
}

void Player::AddMaxHp(int32_t maxHp)
{
	changeMaxHp_ += float(maxHp) / 100.0f;
}

void Player::AddCost(int32_t cost)
{
	nowCost += cost;
}

void Player::Draw()
{
	PlayerBulletManager::Instance()->Draw();

	float leftPos = pos_.x - drawSize_.x / 2;
	float rightPos = pos_.x + drawSize_.x / 2;
	float upPos = pos_.y - drawSize_.y / 2;
	float downPos = pos_.y + drawSize_.y / 2;

	DrawBox(leftPos,upPos,rightPos,downPos,GetColor(255,255,255),true);
	DrawBox(pos_.x - hitboxSize_.x / 2,pos_.y - hitboxSize_.y / 2,pos_.x + hitboxSize_.x/2,pos_.y + hitboxSize_.y/2,GetColor(255,0,0),false);
	//向いてる方向の視覚化
	if ( direction_ )
	{
		DrawBox(rightPos,upPos,rightPos - 5,upPos + 5,GetColor(255,0,0),true);
	} 
	else
	{
		DrawBox(leftPos,upPos,leftPos + 5,upPos + 5,GetColor(255,0,0),true);
	}

	if ( attackZ_ != nullptr )
	{
		attackZ_->Draw();
	}

	if ( attackX_ != nullptr )
	{
		attackX_->Draw();
	}
}

void Player::Load()
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

	topSpeed_= jsonObject[ "TopSpeed" ];
	acceleration_= jsonObject[ "Acceleration" ];
	airAcceleration_ = jsonObject[ "AirAcceleration" ];
	deccelaration_ = jsonObject[ "Deccelaration" ];
	airDeccelaration_ = jsonObject[ "AirDeccelaration" ];
	gravityAcceleration_ = jsonObject[ "GravityAcceleration" ];
	jumpAcceleration_ = jsonObject[ "JumpAcceleration" ];
	jumpInitialVelocity_ = jsonObject[ "JumpInitialVelocity" ];
}
