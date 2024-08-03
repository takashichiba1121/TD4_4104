#include "HealObject.h"
#include "GameConfig.h"
#include "DxlibInclude.h"
#include "Player.h"

void HealObject::Initialize()
{
	shape_ = new RectShape();
	shape_->SetRadius(drawSize_ / 2);
	pos_.x = GameConfig::GetWindowWidth() / 2;
	pos_.y = GameConfig::GetWindowHeight() - 128;
	drawSize_ = { 32,32 };
	hp_ = 1;
	islive_ = true;
	healPower = 25;
}

void HealObject::Update()
{

	shape_->SetCenter({ pos_.x , pos_.y });
}

void HealObject::Draw()
{
	DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.y / 2,pos_.x + drawSize_.x / 2,pos_.y + drawSize_.y / 2,0x00f100,true);
}


//void HealObject::OnCollision()
//{
//	if ( GetCollisionInfo().userData )
//	{
//		if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
//		{
//			dynamic_cast< Player* >( GetCollisionInfo().object )->;
//		}
//	}
//}
