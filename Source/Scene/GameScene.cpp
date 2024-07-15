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
	player_->Initialize();
	
	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Initialize();

	enemys_ = std::make_unique<EnemyManager>();
	enemys_->Initialize();
	enemys_->SetPlayerPtr(player_.get());

	nodeManager_ = NodeManager::GetInstance();
	nodeManager_->SetMapChip(mapChip_.get());
	nodeManager_->SetPlayer(player_.get());
	nodeManager_->Initialize();
	nodeManager_->StartNodeSet(0);
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	if ( Input::Instance()->TriggerKey(KEY_INPUT_R) )
	{
		nodeManager_->Reset();
	}

	nodeManager_->Update();

	player_->Update();
	enemys_->Update();


	CollisionManager::GetInstance()->Update();
}

void GameScene::Draw()
{
	mapChip_->Draw({0,0});
	
	player_->Draw();
	enemys_->Draw();

	nodeManager_->NodeDrew();
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
