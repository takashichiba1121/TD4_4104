#include "HealObject.h"
#include "GameConfig.h"
#include "DxlibInclude.h"
#include "Player.h"
#include <CollisionManager.h>

void HealObject::Initialize()
{
	drawSize_ = { 32,32 };
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	SetShape(shape_);
	CollisionManager::GetInstance()->AddObject(this);
	textureId_ = LoadGraph("Resources\\healBall.png");
	pos_.x = GameConfig::GetWindowWidth() / 2;
	pos_.y = GameConfig::GetWindowHeight() - 128;
	tag.tag = "Heal";
	userData_ = &tag;



	hp_ = 1;
	heal = true;
	healPower = 25;
}

void HealObject::Update()
{
	if ( heal )
	{
		shape_->SetCenter({ pos_.x , pos_.y });
	}
}

void HealObject::Draw()
{
	if ( heal )
	{
		DrawRotaGraph(pos_.x,pos_.y,1,0,textureId_,true);
	}
}


void HealObject::OnCollision()
{
	if ( GetCollisionInfo().userData )
	{
		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
		{
			dynamic_cast< Player* >( GetCollisionInfo().object )->Heel(healPower);
			heal = false;
		}
	}
}
