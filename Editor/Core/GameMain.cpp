#include "GameMain.h"

#include"SceneFactrory.h"

void GameMain::Initialize()
{
	FrameWork::Initialize();

	SceneManager::GetInstance()->SetSceneFactory(SceneFactrory::GetInstance());

	SceneManager::GetInstance()->ChangeScene("EDITOR");
}

void GameMain::Finalize()
{
	FrameWork::Finalize();
}

void GameMain::Update()
{
	FrameWork::Update();
}

void GameMain::UIUpdate()
{
	FrameWork::UIUpdate();
}

void GameMain::EditorDraw()
{
	FrameWork::EditorDraw();
}

void GameMain::UIDraw()
{
	FrameWork::UIDraw();
}

bool GameMain::UserEndRequst()
{
	return false;
}
