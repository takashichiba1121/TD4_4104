#include "BossEnemy.h"
#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include<Collision.h>
#include<GameConfig.h>

void BossEnemy::Initialize()
{
	drawSize_ = { 192,192 };


	attack_ = std::make_unique<BossPunchAttack>();

	{
		GameConfig::Boss* bossConfig = GameConfig::GetBossConfig();

		{
			hp_ = bossConfig->hp;
			attackInterval_ = 60 * bossConfig->attackInterval;
			approachHitBox_.SetRadius({ float(bossConfig->approachHitBoxX),float( bossConfig->approachHitBoxY ) });
		}
		{
			attack_->SetTime(bossConfig->attack.time);
			attack_->SetSize({ float(bossConfig->attack.sizeX),float(bossConfig->attack.sizeY) });
			attack_->SetPower(bossConfig->attack.power);
		}
	}

	attack_->SetBossSize({ drawSize_ });
	attack_->Initialize();

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

	switch ( phase_ )
	{
	case APPROACH:

		ApproachMove();

		break;
	case ATTACK:
		AttackMove();
		break;
	default:
		break;
	}

	SetMapChipSpeed({ velocity_ * speed_,gravity_ });
	shape_->SetCenter(pos_);
}

void BossEnemy::Draw()
{
	if ( !islive_ ) return;
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2,
		pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,GetColor(155,0,0),true);

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

	switch ( phase_ )
	{
	case APPROACH:
		DrawBox(approachHitBox_.GetCenter().x - approachHitBox_.GetRadius().x,approachHitBox_.GetCenter().y - approachHitBox_.GetRadius().y,
			approachHitBox_.GetCenter().x + approachHitBox_.GetRadius().x,approachHitBox_.GetCenter().y + approachHitBox_.GetRadius().y,
			GetColor(255,255,255),true);
		break;
	case ATTACK:
		attack_->Draw();
		break;
	default:
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

}

void BossEnemy::ApproachMove()
{
	velocity_.x = PlayerDir();

	playerRect_.SetCenter(playerPtr_->GetPos());
	approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });

	if ( Collision::Rect2Rect(playerRect_,approachHitBox_) )
	{
		phase_ = Phase::ATTACK;
		velocity_.x = 0;
	}
}

void BossEnemy::AttackMove()
{
	if ( attackInterval_ == 0 )
	{
		attackInterval_ = -1;
		attack_->SetDir(int32_t(PlayerDir()));
		Attack();
	}
	else if ( attackInterval_ > 0 )
	{
		attackInterval_--;
	}

	if ( phase_ == Phase::ATTACK )
	{
		attack_->Update();
		attack_->SetBossPos(pos_);

	}
}

void BossEnemy::Attack()
{
	if ( phase_ == Phase::ATTACK )
	{
		attack_->Attack();
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

