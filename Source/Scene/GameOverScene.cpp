#include "GameOverScene.h"
#include<DxlibInclude.h>
#include<SceneManager.h>

void GameOverScene::Initialize()
{
	input_ = Input::Instance();
	assert(input_);
	SetFontSize(40);

	backGround_ = LoadGraph(std::string("Resources/Result/gameover.png"));
}

void GameOverScene::Update()
{
	if ( input_->TriggerKey(KEY_INPUT_RETURN)||input_->TriggerPadKey(PAD_INPUT_1) )
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	DrawGraph(0,0,backGround_,true);

	DrawFormatString(470,450,GetColor(255,255,255),"Title To EnterKye or A");
}

void GameOverScene::SpriteDraw()
{
}

void GameOverScene::Finalize()
{
	SetFontSize(20);
}

