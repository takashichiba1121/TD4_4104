#include "GameScene.h"

#include"imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"
#include"Input.h"
#include"PlayerBulletManager.h"
#include"GameConfig.h"

#include<SceneManager.h>

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

	powerUp_ = std::make_unique<PowerUpCave>();
	powerUp_->Initialize();
	powerUp_->SetPlayer(player_.get());
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	if ( Input::Instance()->TriggerKey(KEY_INPUT_R) )
	{
		nodeManager_->Reset();
	}

	nodeManager_->Update();

	if ( player_->IsPowerUp() )
	{
		powerUp_->Update();

		uint32_t powerUpNum=player_->PowerUp();

		powerUp_->SetSlect(powerUpNum);
		if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) )
		{
			powerUp_->StatusChenge();

			player_->EndPowerUp();
		}
	}
	else
	{
		player_->Update();
	}

	enemys_->Update();


	CollisionManager::GetInstance()->Update();

	//TODO
	if ( enemys_->GameEnd() )
	{
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	//TODO
	if ( player_->GetHp() <= 0 )
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GameScene::Draw()
{
	DrawGraph(0,0,backGround_,true);

	mapChip_->Draw({ 0,0 });

	player_->Draw();
	enemys_->Draw();

	powerUp_->Draw();

	nodeManager_->NodeDrew(100,600);
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
	PlayerBulletManager::Instance()->Clear();
}
