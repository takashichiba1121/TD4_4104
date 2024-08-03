#include "BossChargeAttack.h"
#include <Util.h>
#include<CollisionConfig.h>
#include<CollisionManager.h>
#include<DxlibInclude.h>
#include<Player.h>

void BossChargeAttack::Attack()
{
	isAttack_ = true;
	velocity_.x = -dir_;
	velocity_.y = 0;
	time_ = 0;
	posY_ = pos_.y - 9;
	pos_.y = posY_;

	shape_->SetCenter(pos_);
	CollisionEnable();
	MapChipObjectEnable();
}

void BossChargeAttack::Initialize()
{
	chargeImg_ = LoadGraph(std::string("Resources/Enemy/chargeBoss.png"));
	effectImg_ = LoadGraph(std::string("Resources/Enemy/tackleEffect.png"));

	int32_t graphSizeX;
	int32_t graphSizeY;
	GetGraphSize(chargeImg_,&graphSizeX,&graphSizeY);

	animeNum_ = graphSizeX / 192;

	GetGraphSize(effectImg_,&graphSizeX,&graphSizeY);

	anime2Num_ = graphSizeX / 192;

	drawSize_ = { 192,192 };

	shape_ = new RectShape();
	shape_->SetRadius(size_ / 2);
	SetShape(shape_);

	userData_ = &tag;

	CollisionDisable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ size_.x / 2,size_.y / 2 });

	MapChipObjectEnable();
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	MapChipObjectDisable();
}

void BossChargeAttack::Update()
{
	AnimeUpdate();

	if ( isAttack_ )
	{
		time_++;

		if ( TIME < time_ )
		{
			CollisionDisable();
			MapChipObjectDisable();

			isAttack_ = false;
			time_ = 0;
		}
		else if ( GetOnDir() & OnDir::LEFT || GetOnDir() & OnDir::RIGHT )
		{
			CollisionDisable();
			MapChipObjectDisable();

			isAttack_ = false;
			time_ = 0;
		}

		speed_ = InQuad(SPEED,0,TIME,time_);

		pos_.y = posY_;
		SetMapChipSpeed({ velocity_ * speed_ });
		shape_->SetCenter(pos_);
	}
}

void BossChargeAttack::Draw()
{
	DrawRectGraphF(shape_->GetCenter().x - drawSize_.x / 2,shape_->GetCenter().y - drawSize_.y / 2,0 + drawSize_.x * anime_,0,drawSize_.x,drawSize_.y,chargeImg_,true,dir_ == 1);

	if ( isAttack_ )
	{
		DrawRectGraphF(shape_->GetCenter().x - drawSize_.x / 2,shape_->GetCenter().y - drawSize_.y / 2,0 + drawSize_.x * anime2_,0,drawSize_.x,drawSize_.y,effectImg_,true,dir_ == 1);
	}
}

void BossChargeAttack::DebugDraw()
{
	DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
	shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
	GetColor(255,0,255),true);
}

void BossChargeAttack::Preparation()
{
	isPreparation_ = true;
	pos_.y -= 9;
	shape_->SetCenter(pos_);
}

void BossChargeAttack::SetBossPos(const Vector2& pos)
{
	pos_ = pos;
}

void BossChargeAttack::SetTime(int32_t time)
{
	TIME  = time;
}

void BossChargeAttack::SetSize(const Vector2& size)
{
	size_ = size;
}

void BossChargeAttack::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossChargeAttack::SetPower(int32_t power)
{
	attackPower_ = power;
}

bool BossChargeAttack::IsAttack() const
{
	return isAttack_;
}

void BossChargeAttack::SetSpeed(float speed)
{
	SPEED = speed_ = speed;
}

void BossChargeAttack::SetAnimeFrame(int32_t frame)
{
	animeFrame_ = frame;
}

void BossChargeAttack::SetAnime2Frame(int32_t frame)
{
	anime2Frame_ = frame;
}

const Vector2& BossChargeAttack::GetPos() const
{
	return pos_;
}

void BossChargeAttack::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}

void BossChargeAttack::AnimeUpdate()
{
	animeTimer_++;

	if ( animeTimer_ == animeFrame_ )
	{
		animeTimer_ = 0;
		anime_++;
		if ( anime_ == animeNum_ )
		{
			anime_ = 0;
		}
	}

	anime2Timer_++;

	if ( anime2Timer_ == anime2Frame_ )
	{
		anime2Timer_ = 0;
		anime2_++;
		if ( anime2_ == anime2Num_ )
		{
			anime2_ = 0;
		}
	}
}
