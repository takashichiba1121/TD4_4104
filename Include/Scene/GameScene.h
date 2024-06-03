#pragma once
#include<memory>
#include<vector>

#include"BaseScene.h"
#include"Player.h"
#include "WalkEnemy.h"
#include"CollisionManager.h"

#include"MapChip.h"

class GameScene : public BaseScene
{
private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<WalkEnemy> testenemy_;//test用、Manager作成後削除
	std::unique_ptr<WalkEnemy> testenemy2_;//test用、Manager作成後削除

	std::unique_ptr<MapChip>mapChip_;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

