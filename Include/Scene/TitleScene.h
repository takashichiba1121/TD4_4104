#pragma once
#include<BaseScene.h>
#include<Input.h>
#include<Vector2.h>
#include "Util.h"
#include "GameConfig.h"

class TitleScene : public BaseScene
{
private:

	Input* input_ = nullptr;

	int32_t titleTex_;
	int32_t mistTex_;
	int32_t backTex_;
	Counter alpha_;
	Counter alphaChengeTimer_;
	float maxAlpha_ = 200;
	float minAlpha_ = 100;
	const float ALPHACHENGETIME_ = 240;
	bool downAlpha_ = false;

	Vector2 mistPos_;
	Vector2 mistPosOffSet_;
	const int32_t WIDHT_ = 15;
	Counter moveTime_;
	int32_t moveCount_ = 70;

	bool isLeft_;
public:
	TitleScene() = default;
	~TitleScene() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void Finalize() override;
};

