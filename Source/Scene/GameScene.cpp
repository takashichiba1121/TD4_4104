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

	powerUp_ = std::make_unique<PowerUpCave>();
	powerUp_->Initialize();
	powerUp_->SetPlayer(player_.get());

	dealer_ = std::make_unique<DealDaemon>();
	dealer_->SetPlayer(player_.get());
	dealer_->Initialize();


	nodeManager_ = NodeManager::GetInstance();
	nodeManager_->SetMapChip(mapChip_.get());
	nodeManager_->SetPlayer(player_.get());
	nodeManager_->SetPowerUp(powerUp_.get());
	nodeManager_->Initialize();
	nodeManager_->StartNodeSet(0);
	backGround_ = LoadGraph("Resources/BackGround/BackGround.png");
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
	else if ( player_->IsChangeParts() )
	{
		dealer_->Update();

		uint32_t powerUpNum = player_->PowerUp();

		dealer_->SetSlect(powerUpNum);

		if ( Input::Instance()->TriggerKey(KEY_INPUT_RETURN) )
		{
			dealer_->Deal();
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) )
		{
			dealer_->PartsChenge();

			player_->EndChangeParts();
		}
	}
	else
	{
		nodeManager_->Update();

		player_->Update();

		CollisionManager::GetInstance()->SetScreenPos(mapChip_->GetScreenPos());
		CollisionManager::GetInstance()->Update();

	//TODO
	//if ( enemys_->GameEnd())
	//{
	//	SceneManager::GetInstance()->ChangeScene("CLEAR");
	//}

		//TODO
		if ( player_->GetHp() <= 0 )
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}
}

void GameScene::Draw()
{
	DrawGraph(0,0,backGround_,true);
	mapChip_->Draw({ 0,0 });
	nodeManager_->Draw();
	player_->Draw();

	//if (!chenged) powerUp_->Draw();

	nodeManager_->NodeMapDraw();

	nodeManager_->Draw();
	
	DrawFormatString(0,0,0xffffff,"MOVE:ARROWKEYorAD");
	DrawFormatString(0,20,0xffffff,"JUMP:SPACE");
	DrawFormatString(0,40,0xffffff,"ATTACK:Z X");
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
	PlayerBulletManager::Instance()->Clear();
}
