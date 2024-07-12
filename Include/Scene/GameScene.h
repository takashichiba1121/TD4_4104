#pragma once
#include<memory>
#include<vector>

#include"BaseScene.h"
#include"Player.h"
#include "EnemyManager.h"
#include"CollisionManager.h"
#include "PowerUpCave.h"

#include"MapChip.h"

class GameScene : public BaseScene
{
private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<EnemyManager> enemys_;

	std::unique_ptr<MapChip>mapChip_;
	std::unique_ptr<PowerUpCave>powerUp_;

	uint32_t backGround_;

	bool isPowerUp=true;

	uint32_t powerUpNum=0;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

