#include "WalkEnemy.h"

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


}
