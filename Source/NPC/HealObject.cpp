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
	CollisionManager::GetInstance()->AddObject(this);
	textureId_ = LoadGraph("Resources\\healBall.png");
	pos_.x = GameConfig::GetWindowWidth() / 2;
	pos_.y = GameConfig::GetWindowHeight() - 128;
	hp_ = 1;
	islive_ = true;
	healPower = 25;
}

void HealObject::Update()
{
	if ( islive_ )
	{
		shape_->SetCenter({ pos_.x , pos_.y });
	}
}

void HealObject::Draw()
{
	if ( islive_ )
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
			islive_ = false;
		}
	}
}
