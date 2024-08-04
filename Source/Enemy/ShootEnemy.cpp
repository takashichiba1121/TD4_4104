#include "ShootEnemy.h"
#include"DxlibInclude.h"
#include "CollisionManager.h"
#include "Player.h"
#include "MapChip.h"
#include "EnemyManager.h"
#include "Collision.h"

using namespace std;
ShootEnemy::~ShootEnemy()
{
	bullets.clear();
}
void ShootEnemy::Initialize()
{
	animeNum_ = 4;
	animeSpeed_ = 10;
	drawSize_ = { 128,128 };
	hitboxSize_ = { 64,128 };

	MapChipObjectEnable();
	SetMapChipCenter(&pos_);
	SetMapChipRadius({ hitboxSize_.x / 2,hitboxSize_.y / 2-5 });

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
	tag.tag = "Enemy";
	userData_ = &tag;

	islive_ = true;

	searchArea_ = make_unique<RectShape>();
	searchArea_->SetRadius({ ( hitboxSize_.x * 5 / 2 ),hitboxSize_.y / 2 - 10 });
	shape_ = new RectShape();
	shape_->SetRadius(hitboxSize_ / 2);
	SetShape(shape_);
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);
	CollisionManager::GetInstance()->AddObject(this);
	attackPower_ = 85;

	attackInterval_ = 90;
	beforeAttackFrame_ = 10;
	attackFrame_ = 25;
	maxHp_ = hp_;
	hp_ = 70;


	tex_ = EnemyManager::GetTexHandle("shootMove");
}

void ShootEnemy::Update()
{
	if ( !islive_ || !playerPtr_ ) return;
	immortalTime_--;
	attackIntervalCounter_.CountUp();
	isCursedDamage_ = false;

	searchArea_->SetCenter({ ( sign(-velocity_.x) * searchArea_->GetRadius().x ) + pos_.x,pos_.y });


	if ( Collision::Rect2Rect(*dynamic_cast< RectShape* >( playerPtr_->GetShape() ),*searchArea_.get()))
	{
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
			tex_ = EnemyManager::GetTexHandle("shootMove");
			break;
		case ATTACK:
			tex_ = EnemyManager::GetTexHandle("shootAttack");
			Attack();
			break;
		default:
			break;
		}
		
	}
	shape_->SetCenter({ pos_.x , pos_.y });

	for ( auto& itr : bullets )
	{
		itr->Update();
	}
	bullets.remove_if([ ] (unique_ptr<EnemyBullet>& bullet)
	{
		return bullet->IsLive() == false;
	});

	bool loop = ( actionMode != ATTACK );
	AnimeUpdate(loop);
	EffectUpdate();
}

void ShootEnemy::Move()
{
	animeNum_ = 4;
	velocity_.Normalize();

	gravity_.y += 0.5f;
	gravity_.y = max(gravity_.y,4);

	if ( GetOnDir() & 0b1 << OnDir::RIGHT | OnDir::LEFT )
	{
		velocity_ *= -1;
	}

	if ( GetOnDir() & 0b1 << OnDir::BOTTOM )
	{
		gravity_ = { 0,0 };
		prevElement_ = mapchip_->GetPosElement(pos_.x,pos_.y + ( hitboxSize_.y / 2 ) + 64);
	}
	Vector2 nextPos_ = { pos_.x + ( ( velocity_.x * speed_ ) ) + ( ( hitboxSize_.x / 2 + 32 ) * -sign(velocity_.x) ),pos_.y + ( hitboxSize_.y / 2 ) };
	nextElement_ = mapchip_->GetPosElement(static_cast< int32_t >( nextPos_.x ),static_cast< int32_t >( nextPos_.y ) + 64);
	if ( ( nextElement_ == NEXT || ( nextElement_ == NONE && ( prevElement_ != NONE && prevElement_ != NEXT ) ) )
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


void ShootEnemy::Attack()
{
	animeNum_ = 1;
	anime_ = 0;
	if ( attackIntervalCounter_.IsCountEnd() )
	{
		if ( beforeAttackCounter_.IsCountEnd() && attackCounter_.IsCountEnd() )
		{
			beforeAttackCounter_.SetEndCount(beforeAttackFrame_);
			attackCounter_.SetEndCount(attackFrame_);
			shootReady_ = true;
		}
		tex_ = EnemyManager::GetTexHandle("shootAttack");
	}


	if ( !beforeAttackCounter_.IsCountEnd() )
	{
		beforeAttackCounter_.CountUp();
		tex_ = EnemyManager::GetTexHandle("shootAttackBefore");
		if ( !beforeAttackSoundPlayed_ )
		{
			beforeAttackSoundPlayed_ = true;
			PlaySoundMem(EnemyManager::GetSoundHandle("shootBeforeAttack"),DX_PLAYTYPE_BACK);
		}
	}
	else if ( !attackCounter_.IsCountEnd() )
	{
		attackCounter_.CountUp();

	}

	if(shootReady_)
	{
		unique_ptr<EnemyBullet> bullet = make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetMapChip(mapchip_);
		bullet->SetVelocity(Vector2(pos_,playerPtr_->GetPos()));
		bullet->SetPos(pos_);
		bullets.push_back(move(bullet));
		attackIntervalCounter_.SetEndCount(attackInterval_);
		actionMode = MOVE;
		shootReady_ = false;
		beforeAttackSoundPlayed_ = false;
		PlaySoundMem(EnemyManager::GetSoundHandle("shootAttack"),DX_PLAYTYPE_BACK);
		anime_ = 0;
		tex_ = EnemyManager::GetTexHandle("shootAttack");
	}
	SetMapChipSpeed({ 0.f,gravity_.y });



}

void ShootEnemy::Draw(Vector2 scroll)
{
	if ( !islive_ ) return;
	bool flag = false;
	if ( velocity_.x > 0 )
	{
		flag = true;
	}
	if ( immortalTime_ <= 0 || immortalTime_ % 3 != 0 )
	{
		DrawRectRotaGraph(pos_.x + scroll.x,pos_.y + scroll.y,drawSize_.x * anime_,0,drawSize_.x,drawSize_.y,1,0,tex_,true,flag);
	}
	for ( auto& itr : bullets )
	{
		itr->Draw(scroll);
	}


	if ( playerPtr_->GetEyeTag() == PlayerEyeTags::Clairvoyance )
	{
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		pos_.x + drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),false);
		int32_t r = pos_.x + drawSize_.x / 2;
		DrawBox(pos_.x - drawSize_.x / 2,pos_.y - drawSize_.x / 2 - hpBerOffSet_,
		 r * ( hp_ / maxHp_ ),pos_.y - drawSize_.x / 2 - hpBerOffSetUnder_,GetColor(155,0,155),true);
	}

}

void ShootEnemy::OnCollision()
{
	//if ( GetCollisionInfo().userData )
	//{
	//	if ( static_cast< UserData* >( GetCollisionInfo().userData )->tag == "Player" )
	//	{
	//		dynamic_cast< Player* >( GetCollisionInfo().object )->Damage(attackPower_);
	//	}
	//}
}


