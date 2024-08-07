#include "BossPunchAttack.h"
#include<CollisionConfig.h>
#include<CollisionManager.h>
#include<DxlibInclude.h>
#include "Player.h"

void BossPunchAttack::Attack()
{
	isAttack_ = true;
	CollisionEnable();
	time_ = TIME;

	isPreparation_ = false;
}

void BossPunchAttack::Initialize()
{

	rightHandImg_ = LoadGraph(std::string("Resources/Enemy/rightHand.png"));

	shape_ = new RectShape();
	shape_->SetRadius(size_ / 2);
	SetShape(shape_);

	userData_ = &tag;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);

	CollisionDisable();
}

void BossPunchAttack::Update()
{
	if ( isPreparation_ )
	{
		shape_->SetCenter({ bossPos_.x + offset_.x + size_.x / 2 * -dir_ , bossPos_.y + offset_.y });
	}

	if ( isAttack_ )
	{
		time_--;

		if ( 0 > time_ )
		{
			isAttack_ = false;
			CollisionDisable();
		}

		shape_->SetCenter({ bossPos_.x + offset_.x + ( ( bossSize_.x / 2 + size_.x / 2 ) * -dir_ ), bossPos_.y + offset_.y });
	}
}

void BossPunchAttack::Draw()
{
	if ( dir_ == 1 )
	{
		DrawRotaGraph(shape_->GetCenter().x,shape_->GetCenter().y,1.0,3.14 / 4,rightHandImg_,true,true);
	}
	else
	{
		DrawRotaGraph(shape_->GetCenter().x,shape_->GetCenter().y,1.0,-3.14 / 4,rightHandImg_,true,false);
	}
	
}

void BossPunchAttack::DebugDraw()
{
	DrawBox(shape_->GetCenter().x - shape_->GetRadius().x,shape_->GetCenter().y - shape_->GetRadius().y,
	shape_->GetCenter().x + shape_->GetRadius().x,shape_->GetCenter().y + shape_->GetRadius().y,
	GetColor(255,255,255),true);
}

void BossPunchAttack::SetBossPos(const Vector2& pos)
{
	bossPos_ = pos;
}

void BossPunchAttack::SetBossSize(const Vector2& size)
{
	bossSize_ = size;
}

void BossPunchAttack::SetTime(int32_t time)
{
	time_ = TIME = time;
}

void BossPunchAttack::SetSize(const Vector2& size)
{
	size_ = size;
}

void BossPunchAttack::SetDir(int32_t dir)
{
	dir_ = dir;
}

void BossPunchAttack::SetPower(int32_t power)
{
	attackPower_ = power;
}

void BossPunchAttack::SetOffset(const Vector2& offset)
{
	offset_ = offset;
}

void BossPunchAttack::Preparation()
{
	isPreparation_ = true;
	shape_->SetCenter({ bossPos_.x + offset_.x + size_.x / 2 * -dir_ , bossPos_.y + offset_.y });
}

bool BossPunchAttack::IsAttack() const
{
	return isAttack_ || isPreparation_;
}

void BossPunchAttack::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
		}
	}
}

BossPunchAttack::~BossPunchAttack()
{
}
