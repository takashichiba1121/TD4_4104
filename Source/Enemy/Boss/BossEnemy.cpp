#include "BossEnemy.h"
#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include<Collision.h>
#include<GameConfig.h>

void BossEnemy::Initialize()
{
	textureId_ = LoadGraph("Resources/Enemy/boss.png");

	int32_t graphSizeX;
	int32_t graphSizeY;
	GetGraphSize(textureId_,&graphSizeX,&graphSizeY);

	animeNum_ = graphSizeX / 192;

	drawSize_ = { 192,192 };

	punch_ = std::make_unique<BossPunchAttack>();
	charge_ = std::make_unique<BossChargeAttack>();

	{
		GameConfig::Boss* bossConfig = GameConfig::GetBossConfig();

		{
			hp_ = bossConfig->hp;
			attackInterval_ = ATTACK_INTERVAL = 60 * bossConfig->attackInterval;
			approachHitBox_.SetRadius({ float(bossConfig->approachHitBoxX),float(bossConfig->approachHitBoxY) });
		}
		{
			punch_->SetTime(bossConfig->attack.time);
			punch_->SetSize({ float(bossConfig->attack.sizeX),float(bossConfig->attack.sizeY) });
			punch_->SetPower(bossConfig->attack.power);
		}

		{
			charge_->SetTime(bossConfig->charge.time);
			charge_->SetSize({ float(bossConfig->charge.sizeX),float(bossConfig->charge.sizeY) });
			charge_->SetPower(bossConfig->charge.power);
			charge_->SetSpeed(bossConfig->charge.speed);
		}
	}


	punch_->SetBossSize({ drawSize_ });
	punch_->Initialize();

	charge_->Initialize();

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ drawSize_.x / 2,drawSize_.y / 2 });

	gravity_ = { 0,1 };
	speed_ = GetRand(4) + 1;
	user.tag = "BossEnemy";
	userData_ = &user;

	islive_ = true;
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 1;


	approachHitBox_.SetCenter(pos_);
	playerRect_.SetRadius(playerPtr_->GetDrawSize());
}

void BossEnemy::Update()
{
	AnimeUpdate();

	Move();
}

void BossEnemy::Move()
{
	velocity_.Normalize();

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
	}

	if ( phase_ == APPROACH )
	{
		ApproachMove();

		SetMapChipSpeed({ velocity_ * speed_,gravity_ });
		shape_->SetCenter(pos_);
	}
	else
	{
		AttackMove();
	}
}

void BossEnemy::Draw()
{
	DrawRectGraphF(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.y / 2,
		 0 + drawSize_.x * anime_,0,
		drawSize_.x,drawSize_.y,
		textureId_,true,playerDir_ == 1);

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG
}

void BossEnemy::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}

void BossEnemy::DebugDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);

	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
	pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

	switch ( phase_ )
	{
	case APPROACH:
		DrawBox(approachHitBox_.GetCenter().x - approachHitBox_.GetRadius().x,approachHitBox_.GetCenter().y - approachHitBox_.GetRadius().y,
			approachHitBox_.GetCenter().x + approachHitBox_.GetRadius().x,approachHitBox_.GetCenter().y + approachHitBox_.GetRadius().y,
			GetColor(255,255,255),true);
		break;
	case PUNCH:
		punch_->Draw();
		break;
	case CHARGE:
		break;
	default:
		break;
	}
	charge_->Draw();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

}

void BossEnemy::ApproachMove()
{
	playerDir_ = velocity_.x = PlayerDir();

	playerRect_.SetCenter(playerPtr_->GetPos());
	approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });

	if ( Collision::Rect2Rect(playerRect_,approachHitBox_) )
	{
		phase_ = Phase::CHARGE;
		velocity_.x = 0;
	}
}

void BossEnemy::AttackMove()
{
	if ( attackInterval_ == 0 )
	{
		playerDir_ = PlayerDir();
		attackInterval_ = -1;
		switch ( phase_ )
		{
		case PUNCH:
			punch_->SetDir(playerDir_);
			break;
		case CHARGE:
			charge_->SetDir(playerDir_);
			break;
		}

		Attack();
	}
	else if ( attackInterval_ > 0 )
	{
		attackInterval_--;
	}
	else
	{
		switch ( phase_ )
		{
		case PUNCH:
			punch_->Update();
			punch_->SetBossPos(pos_);

			if ( !punch_->IsAttack() )
			{
				phase_ = APPROACH;
				attackInterval_ = ATTACK_INTERVAL;
				approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });
			}
			break;
		case CHARGE:
			charge_->Update();

			pos_ = charge_->GetPos();

			if ( !charge_->IsAttack() )
			{
				phase_ = APPROACH;
				attackInterval_ = ATTACK_INTERVAL;
				approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });
				CollisionEnable();
				MapChipObjectEnable();

			}
			break;
		}
	}
}

void BossEnemy::Attack()
{
	switch ( phase_ )
	{
	case PUNCH:
		punch_->Attack();
		break;
	case CHARGE:
		charge_->Attack();
		CollisionDisable();
		MapChipObjectDisable();
		charge_->SetBossPos(pos_);
		break;
	}
}

float BossEnemy::PlayerDir()
{
	Vector2 playerPos = playerRect_.GetCenter();

	if ( playerPos.x > pos_.x )
	{
		return -1.0f;
	}
	else
	{
		return 1.0f;
	}
}

void BossEnemy::AnimeUpdate()
{
	animeTimer_++;

	if ( animeTimer_ == 60 )
	{
		animeTimer_ = 0;
		anime_++;
		if ( anime_ == animeNum_ )
		{
			anime_ = 0;
		}
	}
}

