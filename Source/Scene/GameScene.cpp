#include "GameScene.h"

#include"imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"
#include"Input.h"

void GameScene::Initialize()
{
	gh = LoadGraph("Resources/Src1.bmp");
}

void GameScene::Update()
{
	ImGui::ShowDemoWindow();
}

void GameScene::Draw()
{
	DrawGraph(0,0,gh,true);
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
