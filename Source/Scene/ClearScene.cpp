#include "ClearScene.h"

#include<DxlibInclude.h>
#include<SceneManager.h>

void ClearScene::Initialize()
{
	input_ = Input::Instance();
	assert(input_);
	SetFontSize(40);

	backGround_ = LoadGraph(std::string("Resources/Result/gameclear.png"));
}

void ClearScene::Update()
{
	if ( input_->TriggerKey(KEY_INPUT_RETURN) || input_->TriggerPadKey(PAD_INPUT_1) )
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	DrawGraph(0,0,backGround_,true);

	DrawFormatString(380,450,GetColor(255,255,255),"Title To EnterKey or A");
}

void ClearScene::SpriteDraw()
{
}

void ClearScene::Finalize()
{
	SetFontSize(20);
}
