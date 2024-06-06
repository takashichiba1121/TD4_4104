#pragma once
#include<BaseScene.h>
#include<Input.h>


class TitleScene : public BaseScene
{
private:

	Input* input_ = nullptr;

public:
	TitleScene() = default;
	~TitleScene() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

