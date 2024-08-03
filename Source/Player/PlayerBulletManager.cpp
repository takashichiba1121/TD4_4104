#include "PlayerBulletManager.h"

PlayerBulletManager* PlayerBulletManager::Instance()
{
	static PlayerBulletManager instance;

	return &instance;
}

void PlayerBulletManager::Update()
{
	bullets_.remove_if([ ] (std::unique_ptr<PlayerBullet>& bullet)
 {
	 return bullet->IsDead();
		});

	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Update();
	}
}

void PlayerBulletManager::Draw(Vector2 scroll)
{
	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Draw(scroll);
	}
}

void PlayerBulletManager::AddBullet(std::unique_ptr<PlayerBullet> newBullet)
{
	bullets_.push_back(std::move(newBullet));
}

void PlayerBulletManager::Fin()
{

	bullets_.clear();
}

void PlayerBulletManager::Clear()
{
	bullets_.clear();
}
