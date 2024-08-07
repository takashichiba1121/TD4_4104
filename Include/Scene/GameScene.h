#pragma once
#include<memory>
#include<vector>

#include"BaseScene.h"
#include"Player.h"
#include "EnemyManager.h"
#include"CollisionManager.h"
#include"NodeManager.h"
#include "PowerUpCave.h"
#include "DealDaemon.h"

#include"MapChip.h"


class GameScene : public BaseScene
{
private:

	std::unique_ptr<Player> player_;

	std::unique_ptr<MapChip>mapChip_;
	NodeManager* nodeManager_;
	std::unique_ptr<PowerUpCave>powerUp_;
	std::unique_ptr<DealDaemon>dealer_;
	std::unique_ptr<EnemyManager>enemys_;
	int32_t bgm_;

	uint32_t backGround_;
	uint8_t num;
	bool chenged;

	bool scrollStop = false;
	bool isSelect = false;

	Vector2 nowScroll = { 0,0 };

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
	Vector2 Scroll();
};

