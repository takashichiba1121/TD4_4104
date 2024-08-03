#include "TitleScene.h"

#include<DxlibInclude.h>
#include<SceneManager.h>

void TitleScene::Initialize()
{
	input_ = Input::Instance();
	assert(input_);
	SetFontSize(40);
	titleTex_ = LoadGraph("Resources\\Title\\titleLogo.png");
	mistTex_ = LoadGraph("Resources\\Title\\mist.png");
	backTex_ = LoadGraph("Resources\\Title\\title.png");
	startSound_ = LoadSoundMem("Resources\\Sound\\SFX_title_gameStart.mp3");
	mistPos_ = { GameConfig::GetWindowWidth() / 2.f,GameConfig::GetWindowHeight() / 2.f };
	mistPosOffSet_ = { 0,0 };
	moveTime_.SetEndCount(moveCount_);
	isLeft_ = false;
	alpha_.SetNowCount(maxAlpha_);
	downAlpha_ = true;
}

void TitleScene::Update()
{
	if ( input_->TriggerKey(KEY_INPUT_RETURN) )
	{
		SceneManager::GetInstance()->ChangeScene("GAME");
		PlaySoundMem(startSound_,DX_PLAYTYPE_BACK);
	}

	if ( isLeft_ )
	{
		moveTime_.CountDown();
	}
	else
	{
		moveTime_.CountUp();
	}

	if ( moveTime_.GetCount() >= moveCount_ )
	{
		isLeft_ = true;
	}
	else if ( moveTime_.GetCount() <= -moveCount_ )
	{
		isLeft_ = false;
	}


	mistPosOffSet_.x = moveTime_.GetCount() / static_cast<float>(moveCount_);
	mistPosOffSet_.y = (mistPosOffSet_.x * mistPosOffSet_.x) * ( 1 - (mistPosOffSet_.x*mistPosOffSet_.x) );
	mistPosOffSet_.y = sqrtf(mistPosOffSet_.y);
	if ( (isLeft_ && mistPosOffSet_.x < 0) ||(!isLeft_ && mistPosOffSet_.x >0))
	{
		mistPosOffSet_.y *= -1;
	}

	if ( downAlpha_ )
	{
		alpha_.CountDown((maxAlpha_ - minAlpha_) / ALPHACHENGETIME_);
	}
	else
	{
 		alpha_.CountUp((maxAlpha_ - minAlpha_) / ALPHACHENGETIME_);
	}

	if ( alpha_.GetCount() >= maxAlpha_ )
	{
		downAlpha_ = true;
	}
	else if ( alpha_.GetCount() <= minAlpha_ )
	{
		downAlpha_ = false;
	}

}

void TitleScene::Draw()
{
	DrawRotaGraph(GameConfig::GetWindowWidth()/2,GameConfig::GetWindowHeight()/2,1,0,backTex_,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha_.GetCount());
	DrawRotaGraph(mistPos_.x + mistPosOffSet_.x * WIDHT_, mistPos_.y + mistPosOffSet_.y * WIDHT_,2,0,mistTex_,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawRotaGraph(GameConfig::GetWindowWidth() / 2,200,1,0,titleTex_,TRUE);

	DrawFormatString(470,450,GetColor(255,255,255),"Start To EnterKey");
}

void TitleScene::SpriteDraw()
{
}

void TitleScene::Finalize()
{
	SetFontSize(20);
	DeleteGraph(titleTex_);
	DeleteGraph(mistTex_);
	DeleteGraph(backTex_);
}
