#pragma once
#include<BaseScene.h>
#include<Input.h>


class ClearScene : public BaseScene
{
private:

	Input* input_ = nullptr;
	int32_t backGround_;

public:
	ClearScene() = default;
	~ClearScene() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

