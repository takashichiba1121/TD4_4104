#include "TitleScene.h"
#include<DxlibInclude.h>
#include<SceneManager.h>


void TitleScene::Initialize()
{
	input_ = Input::Instance();
	assert(input_);
	SetFontSize(40);
}

void TitleScene::Update()
{
	if ( input_->TriggerKey(KEY_INPUT_RETURN) )
	{
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	DrawFormatString(460,200,GetColor(255,255,255),"Eternal Damnation");

	DrawFormatString(470,450,GetColor(255,255,255),"Start To EnterKey");
}

void TitleScene::SpriteDraw()
{
}

void TitleScene::Finalize()
{
	SetFontSize(20);
}
