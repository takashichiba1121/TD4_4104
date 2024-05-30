#pragma once
#include<list>
#include<memory>
#include"PlayerBullet.h"
class PlayerBulletManager
{
#pragma region Singleton
private:
	PlayerBulletManager() {};

	~PlayerBulletManager() {};
public:
	PlayerBulletManager(const PlayerBulletManager& carManager) = delete;

	PlayerBulletManager& operator=(const PlayerBulletManager& carManager) = delete;

	static PlayerBulletManager* Instance();
#pragma endregion
public:

	void Update();

	void Draw();

	void AddBullet(std::unique_ptr<PlayerBullet> newBullet);

	void Fin();

	void Clear();

	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() {
		return bullets_;
	}
private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};

