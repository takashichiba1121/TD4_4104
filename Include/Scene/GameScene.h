#pragma once
#include"BaseScene.h"

class GameScene : public BaseScene
{
private:

	int32_t gh = 0;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

