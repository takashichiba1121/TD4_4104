#include "GameScene.h"

#include"imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"
#include"Input.h"

#include<DxLib.h>



void GameScene::Initialize()
{
	player_ = std::make_unique<Player>();
	player_->Initialze();

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Initialize();
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChip());
}

void GameScene::Update()
{
	ImGui::ShowDemoWindow();

	CollisionManager::GetInstance()->Update();

	player_->Update();
}

void GameScene::Draw()
{
	player_->Draw();

	mapChip_->Draw({0,0});
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
