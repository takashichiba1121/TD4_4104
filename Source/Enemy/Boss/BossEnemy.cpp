#include "BossEnemy.h"
#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include<Collision.h>
#include<GameConfig.h>
#include<Input.h>

void BossEnemy::Initialize()
{
	textureId_ = LoadGraph(std::string("Resources/Enemy/boss.png"));
	bodyImg_ = LoadGraph(std::string("Resources/Enemy/bossBody.png"));

	int32_t graphSizeX;
	int32_t graphSizeY;
	GetGraphSize(textureId_,&graphSizeX,&graphSizeY);

	animeNum_ = graphSizeX / 192;

	drawSize_ = { 192,192 };

	punch_ = std::make_unique<BossPunchAttack>();
	charge_ = std::make_unique<BossChargeAttack>();
	longRange_ = std::make_unique<BossLongRangeAttack>();

	{
		GameConfig::Boss* bossConfig = GameConfig::GetBossConfig();

		{
			hp_ = HP = bossConfig->hp;
			attackInterval_ = ATTACK_INTERVAL = 60 * bossConfig->attackInterval;
			attackApproachHitBox_.SetRadius({ float(bossConfig->attackApproachHitBoxX),float(bossConfig->attackApproachHitBoxY) });
			chargeApproachHitBox_.SetRadius({ float(bossConfig->chargeApproachHitBoxX),float(bossConfig->chargeApproachHitBoxY) });
			probabilities[ 0 ] = bossConfig->probabilitie1;
			probabilities[ 1 ] = bossConfig->probabilitie2;
		}
		{
			GameConfig::Boss::Attack& punch = bossConfig->attack;

			punch_->SetTime(punch.time);
			punch_->SetSize({ float(punch.sizeX),float(punch.sizeY) });
			punch_->SetPower(punch.power);
			punch_->SetOffset({ float(punch.offsetX),float(punch.offsetY) });
		}

		{
			GameConfig::Boss::Charge& charge = bossConfig->charge;
			charge_->SetTime(charge.time);
			charge_->SetSize({ float(charge.sizeX),float(charge.sizeY) });
			charge_->SetPower(charge.power);
			charge_->SetSpeed(charge.speed);
			charge_->SetAnimeFrame(charge.animeFrame);
			charge_->SetAnime2Frame(charge.anime2Frame);
		}

		{
			GameConfig::Boss::LongRange& longRange = bossConfig->longRange;

			longRange_->SetChargeTime(longRange.chargeTime);
			longRange_->SetFreezeTime(longRange.freezeTime);

			longRange_->SetBulletTime(longRange.bulletTime);
			longRange_->SetBulletSpeed(longRange.bulletSpeed);
			longRange_->SetBulletSize({ float(longRange.bulletSizeX),float(longRange.bulletSizeY) });
			longRange_->SetBulletPower(longRange.bulletPower);

		}
	}


	punch_->SetBossSize({ drawSize_ });
	punch_->Initialize();

	charge_->Initialize();

	longRange_->Initialize();

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

	playerRect_.SetRadius({ 1,1 });

	distribution = std::discrete_distribution<int>(probabilities,probabilities + 2);

	phase_ = APPROACH;

	approachHitBox_.SetRadius(attackApproachHitBox_.GetRadius());

	hpBerTextureId_ = LoadGraph(std::string("Resources\\UI\\UI_enemy_boss_HP_bar.png"));

	hpBerBackgroundTextureId_ = LoadGraph(std::string("Resources\\UI\\UI_enemy_boss_HP_frame.png"));
}

void BossEnemy::Update()
{
	AnimeUpdate();

	Move();

	if ( Input::Instance()->TriggerKey(KEY_INPUT_UP) )
	{
		hp_--;
	}
}

void BossEnemy::Move()
{
	velocity_.Normalize();
	immortalTime_--;

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
	}

	if ( phase_ == APPROACH )
	{
		SetMapChipSpeed({ velocity_ * speed_,gravity_ });
		shape_->SetCenter(pos_);

		ApproachMove();
	}
	else
	{
		AttackMove();
	}

	if ( immortalTime_ <= 0 )
	{
		immortal_ = false;
	}

	longRange_->BulletUpdate();
}

void BossEnemy::Draw(Vector2 scrool)
{
	if ( immortalTime_ <= 0 || immortalTime_ % 3 != 0 )
	{
		switch ( phase_ )
		{
		case APPROACH:
			DrawRectGraphF(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.y / 2,
			0 + drawSize_.x * anime_,0,
			drawSize_.x,drawSize_.y,
			textureId_,true,playerDir_ == 1);
			break;
		case PUNCH:
			DrawRotaGraph(pos_.x,pos_.y,1.0,0.0,bodyImg_,true,playerDir_ == 1);
			punch_->Draw();
			break;
		case CHARGE:
			charge_->Draw();
			break;
		case LONG_RANGE:

			if ( !longRange_->IsAttack() )
			{
				DrawRotaGraph(pos_.x,pos_.y,1.0,0.0,bodyImg_,true,playerDir_ == 1);

			}

			break;
		default:
			break;
		}
	}

	DrawRotaGraphFast3(120,50,0,0,1.0,1.0,0.0,hpBerBackgroundTextureId_,true);
	DrawRotaGraphFast3(120,50,0,0,float(hp_) / float(HP),1.0,0.0,hpBerTextureId_,true);

	longRange_->Draw();

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

BossEnemy::~BossEnemy()
{

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
		punch_->DebugDraw();
		break;
	case CHARGE:
		charge_->DebugDraw();
		break;
	case LONG_RANGE:
		longRange_->DebugDraw();
		break;
	default:
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

}

void BossEnemy::ApproachMove()
{
	playerDir_ = velocity_.x = PlayerDir();

	playerRect_.SetCenter(playerPtr_->GetPos());
	approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });

	float hpRet = hp_ / float(HP);
	int32_t hp = hpRet * 100;

	if ( hp <= 50 && hp > 25 && longRangeAttackCount_ == 0 ||
		hp <= 25 && hp > 10 && longRangeAttackCount_ == 1 ||
		hp <= 10 && longRangeAttackCount_ == 2 )
	{
		velocity_.x = 0;
		longRangeAttackCount_++;

		nextPhase_ = GetPhase();
		phase_ = LONG_RANGE;
		attackInterval_ = 0;
		SetMapChipSpeed({ playerDir_ * 1000.0f,0 });

		if ( nextPhase_ == PUNCH )
		{
			approachHitBox_.SetRadius(attackApproachHitBox_.GetRadius());
		}
		else if ( nextPhase_ == CHARGE )
		{
			approachHitBox_.SetRadius(chargeApproachHitBox_.GetRadius());
		}
	}
	else
	{
		if ( Collision::Rect2Rect(playerRect_,approachHitBox_) )
		{
			phase_ = nextPhase_;
			nextPhase_ = GetPhase();

			velocity_.x = 0;

			if ( phase_ == PUNCH )
			{
				punch_->SetBossPos(pos_);
				punch_->Preparation();
				punch_->SetDir(playerDir_);
			}
			else if ( phase_ == CHARGE )
			{
				charge_->SetBossPos(pos_);
				charge_->Preparation();
				charge_->SetDir(playerDir_);
			}

			if ( nextPhase_ == PUNCH )
			{
				approachHitBox_.SetRadius(attackApproachHitBox_.GetRadius());
			}
			else if ( nextPhase_ == CHARGE )
			{
				approachHitBox_.SetRadius(chargeApproachHitBox_.GetRadius());
			}
		}
	}
}

void BossEnemy::AttackMove()
{
	if ( phase_ == CHARGE )
	{
		charge_->Update();
	}

	if ( attackInterval_ == 0 )
	{
		playerDir_ = PlayerDir();
		attackInterval_ = -1;

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

			if ( !punch_->IsAttack() )
			{
				phase_ = APPROACH;
				attackInterval_ = ATTACK_INTERVAL;
				approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });
			}
			break;

		case CHARGE:

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

		case LONG_RANGE:

			longRange_->Update();

			if ( !longRange_->IsAttack() )
			{
				phase_ = APPROACH;
				attackInterval_ = ATTACK_INTERVAL;
				approachHitBox_.SetCenter({ pos_.x + ( shape_->GetRadius().x + approachHitBox_.GetRadius().x ) * -velocity_.x,pos_.y });
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
		punch_->SetBossPos(pos_);
		break;
	case CHARGE:
		charge_->SetDir(playerDir_);
		charge_->Attack();
		CollisionDisable();
		MapChipObjectDisable();
		charge_->SetBossPos(pos_);
		break;
	case LONG_RANGE:
		longRange_->SetDir(playerDir_);
		longRange_->SetBossPos(pos_);
		longRange_->Attack();
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

Phase BossEnemy::GetPhase()
{
	Phase ret;

	float hpRet = hp_ / float(HP);
	int32_t hp = hpRet * 100;

	if ( hp < 75 )
	{
		static std::mt19937 gen(std::time(nullptr));
		ret = static_cast< Phase >( distribution(gen) + 1 );
	}
	else
	{
		ret = PUNCH;

	}

	return ret;
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

