#pragma once
#include<BaseScene.h>
#include<Input.h>


class GameOverScene : public BaseScene
{
private:

	Input* input_ = nullptr;
	int32_t backGround_;

public:
	GameOverScene() = default;
	~GameOverScene() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

