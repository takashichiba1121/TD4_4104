#include "WalkEnemy.h"
#include"DxlibInclude.h"
void WalkEnemy::Initialize()
{


}

void WalkEnemy::Update()
{
	velocity.Normalize();
	pos_ += velocity * speed;

}

void WalkEnemy::Draw()
{
	DrawBox(pos_.x,pos_.y,pos_.x + drawSize_.x,pos_.y + drawSize_.y,GetColor(255,255,255),true);

}
