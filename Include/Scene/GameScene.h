#pragma once
#include"BaseScene.h"
#include<memory>
#include"Player.h"

class GameScene : public BaseScene
{
private:

	int32_t gh = 0;

	std::unique_ptr<Player> player_;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

