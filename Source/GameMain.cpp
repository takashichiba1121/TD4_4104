#include "GameMain.h"
#include"SceneFactrory.h"

void GameMain::Initialize()
{
	GameConfig::Load("Resources/Config/Config.json");

	FrameWork::Initialize();

	SceneManager::GetInstance()->SetSceneFactory(SceneFactrory::GetInstance());

	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void GameMain::Finalize()
{
	FrameWork::Finalize();
}

void GameMain::Update()
{
	FrameWork::Update();
}

void GameMain::Draw()
{
	FrameWork::Draw();
}

bool GameMain::UserEndRequst()
{
	return false;
}
