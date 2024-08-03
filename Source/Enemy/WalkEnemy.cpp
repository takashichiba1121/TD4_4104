#include "WalkEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include "MapChip.h"
#include "Collision.h"
#include "EnemyManager.h"

using namespace std;
void WalkEnemy::Initialize()
{

	animeNum_ = 5;
	animeSpeed_ = 10;
	drawSize_ = { 128,128 };
	hitboxSize_ = { 64,128 };

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ hitboxSize_.x / 2,hitboxSize_.y / 2 });

	ternInverval_ = 2;
	gravity_ = { 0,1 };
	originalSpeed_ = GetRand(4) + 1;
	if ( GetRand(2) >= 2 )
	{
		velocity_ = { 1,0 };
	}
	else
	{
		velocity_ = { -1,0 };
	}

	islive_ = true;

	searchArea_ = make_unique<RectShape>();
	searchArea_->SetRadius({ ( hitboxSize_.x * 3 / 2 ),hitboxSize_.y / 2 });
	attackArea_ = make_unique<RectShape>();
	attackArea_->SetRadius({ hitboxSize_.x ,hitboxSize_.y / 2});
	shape_ = new RectShape();
	shape_->SetRadius(hitboxSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 90;
	attackInterval_ = 60;
	beforeAttackFrame_ = 5;
	attackFrame_ = 25;
	maxHp_ = 150;
	hp_ = 150;
	tex_ = EnemyManager::GetTexHandle("adjacentMove");
	tag.tag = "Enemy";
	userData_ = &tag;

}

void WalkEnemy::Update()
{
	if ( !islive_ ||!playerPtr_ ) return;
	immortalTime_--;
	attackIntervalCounter_.CountUp();
	isCursedDamage_ = false;

	searchArea_->SetCenter({ ( sign(-velocity_.x) * searchArea_->GetRadius().x ) + pos_.x,pos_.y });

	attackArea_->SetCenter({ ( sign(-velocity_.x) * attackArea_->GetRadius().x ) + pos_.x,pos_.y });

	if ( Collision::Rect2Rect(*dynamic_cast< RectShape* >( playerPtr_->GetShape() ),*searchArea_.get()) && actionMode != ATTACK )
	{
		actionMode = ENEMYAPPROACH;
	}
	else if ( actionMode != ATTACK )
	{
		actionMode = MOVE;
	}

	if ( Collision::Rect2Rect(*dynamic_cast< RectShape* >( playerPtr_->GetShape() ),*attackArea_.get()) && attackIntervalCounter_.IsCountEnd() )
	{
		if ( actionMode != ATTACK )
		{
			beforeAttackCounter_.SetEndCount(beforeAttackFrame_);
			attackCounter_.SetEndCount(attackFrame_);
		}
		actionMode = ATTACK;
	}

	if ( immortalTime_ <= 0 )
	{
		immortal_ = false;
	}

	if ( IsEffect(DELAY) )
	{
		speed_ = 1;
	}
	else
	{
		speed_ = originalSpeed_;
	}

	if ( !IsEffect(BIND) && !IsEffect(ICED) )
	{
		switch ( actionMode )
		{
		case MOVE:
			Move();
			tex_ = EnemyManager::GetTexHandle("adjacentMove");
			break;
		case ENEMYAPPROACH:
			Approach();
			tex_ = EnemyManager::GetTexHandle("adjacentDash");
			break;
		case ATTACK:
			Attack();
			tex_ = EnemyManager::GetTexHandle("adjacentAttack");
			break;
		default:
			break;
		}
		
	}
	shape_->SetCenter({ pos_.x , pos_.y });
	bool loop = (actionMode != ATTACK);
	AnimeUpdate(loop);

	EffectUpdate();
}

void WalkEnemy::Move()
{

	velocity_.Normalize();

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT)
	{
		velocity_ *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
		prevElement_ = mapchip_->GetPosElement(pos_.x,pos_.y + ( hitboxSize_.y / 2 ) + 64);
	}
	 nextPos_ = { pos_.x + ( ( velocity_.x * speed_ ) ) + ( ( hitboxSize_.x / 2 + 32)* -sign(velocity_.x) ),pos_.y + ( hitboxSize_.y / 2 )};
	 nextElement_ = mapchip_->GetPosElement(static_cast<int32_t>(nextPos_.x),static_cast< int32_t >( nextPos_.y)+64);

	 if ( ( nextElement_ == NEXT || ( nextElement_ == NONE && ( prevElement_ != NONE || prevElement_ != NEXT ) ) )
	 && !tern_ )
	{
		velocity_ *= -1;
		tern_ = true;
	}

	if ( prevElement_ != NONE )
	{
		velocity_.y = 0;
		gravity_.y = 0;
	}


	if ( tern_ )
	{
		ternInvervalTimer_++;
		if ( ternInverval_ < ternInvervalTimer_ )
		{
			tern_ = false;
			ternInvervalTimer_ = 0;
		}
	}


	SetMapChipSpeed({ velocity_ * speed_,gravity_ });
	shape_->SetCenter(pos_);

	
}

void WalkEnemy::Approach()
{

	velocity_ = Vector2(playerPtr_->GetPos(),pos_);
	const float defaultSpeed = speed_;
	speed_ += 1;

	velocity_.Normalize();

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		actionMode = MOVE;
		return;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
		
	}
	prevElement_ = mapchip_->GetPosElement(pos_.x,pos_.y + ( hitboxSize_.y / 2 ) + 18);
	nextElement_ = mapchip_->GetPosElement(pos_.x + ( ( velocity_.x * speed_ ) ) + ( hitboxSize_.x / 2 * sign(velocity_.x) ),
	   pos_.y + ( hitboxSize_.y / 2 ) + 5);

	if ( ( nextElement_ == NEXT || ( nextElement_ == NONE &&(prevElement_ != NONE || prevElement_ != NEXT) ) ) && !tern_ )
	{
		actionMode = MOVE;
		return;
		tern_ = true;
	}

	if ( prevElement_ != NONE )
	{
		velocity_.y = 0;
		gravity_.y = 0;
	}


	if ( tern_ )
	{
		ternInvervalTimer_++;
		if ( ternInverval_ < ternInvervalTimer_ )
		{
			tern_ = false;
			ternInvervalTimer_ = 0;
		}
	}

	SetMapChipSpeed({ velocity_ * speed_,gravity_ });
	speed_ = defaultSpeed;
}


void WalkEnemy::Attack()
{
	
	if ( !beforeAttackCounter_.IsCountEnd() )
	{
		beforeAttackCounter_.CountUp();
	}
	else if ( !attackCounter_.IsCountEnd() )
	{
		if ( !attackSoundPlayed_ )
		{
			PlaySoundMem(EnemyManager::GetSoundHandle("shootAttack"),DX_PLAYTYPE_BACK);
			attackSoundPlayed_ = true;
		}
		attackCounter_.CountUp();
		if ( Collision::Rect2Rect(*dynamic_cast< RectShape* >( playerPtr_->GetShape() ),*attackArea_.get()) )
		{
			playerPtr_->Damage(attackPower_);
		}
		attackIntervalCounter_.SetEndCount(attackInterval_);
	}
	else
	{
		actionMode = MOVE;
		attackSoundPlayed_ = false;
	}
	SetMapChipSpeed({ 0.f,gravity_.y });

}

void WalkEnemy::Draw(Vector2 scroll)
{
	if ( !islive_ ) return;
	bool flag = false;
	if ( velocity_.x > 0 )
	{
		flag = true;
	}
	DrawRectRotaGraph(pos_.x + scroll.x,pos_.y + scroll.y,drawSize_.x * anime_,0,drawSize_.x,drawSize_.y,1,0,tex_,true,flag);

#ifdef _DEBUG
	if ( actionMode == ATTACK )
	{
		DrawBox(attackArea_->GetCenter().x - attackArea_->GetRadius().x + scroll.x,
			attackArea_->GetCenter().y - attackArea_->GetRadius().y + scroll.y,
		attackArea_->GetCenter().x + attackArea_->GetRadius().x + scroll.x,
			attackArea_->GetCenter().y + attackArea_->GetRadius().y + scroll.y
			,GetColor(155,0,0),false);
	}
	else
	{
		DrawBox(searchArea_->GetCenter().x - searchArea_->GetRadius().x + scroll.x,
			pos_.y - searchArea_->GetRadius().y + scroll.y,
		searchArea_->GetCenter().x + searchArea_->GetRadius().x + scroll.x
			,pos_.y + searchArea_->GetRadius().y + scroll.y,GetColor(155,0,0),false);
	}
#endif // DEBUG

	

	if ( playerPtr_->GetEyeTag() == PlayerEyeTags::Clairvoyance )
	{
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		pos_.x + drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),false);
		int32_t r = pos_.x + drawSize_.x / 2;
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		 r * ( hp_ / maxHp_ ),pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),true);
	}

}

void WalkEnemy::OnCollision()
{
	//if ( GetCollisionInfo().userData )
	//{
	//	if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
	//	{
	//		dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
	//	}
	//}
}


