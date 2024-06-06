#pragma once
#include<memory>
#include<vector>

#include"BaseScene.h"
#include"Player.h"
#include "EnemyManager.h"
#include"CollisionManager.h"

#include"MapChip.h"

class GameScene : public BaseScene
{
private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<EnemyManager> enemys_;

	std::unique_ptr<MapChip>mapChip_;

	uint32_t backGround_;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

