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
	mapChip_->MapLoad("Resources/Export/Map/TestMap.json");

	CollisionManager::GetInstance()->SetMapChip(mapChip_->GetMapChip());

	enemys_ = std::make_unique<EnemyManager>();
	enemys_->Initialize();
	enemys_->SetPlayerPtr(player_.get());

	powerUp_ = std::make_unique<PowerUpCave>();
	powerUp_->Initialize();

	backGround_ = LoadGraph("Resources/BackGround/BackGround.png");

}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	if ( Input::Instance()->TriggerKey(KEY_INPUT_R) )
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if ( isPowerUp )
	{
		if ( Input::Instance()->TriggerKey(KEY_INPUT_LEFT) || Input::Instance()->TriggerKey(KEY_INPUT_A) )
		{
			if ( powerUpNum == 0 )
			{
				powerUpNum = 2;
			}
			else
			{
				powerUpNum--;
			}
		}
		if ( Input::Instance()->TriggerKey(KEY_INPUT_RIGHT) || Input::Instance()->TriggerKey(KEY_INPUT_D) )
		{
			powerUpNum++;
			if ( powerUpNum >= 3 )
			{
				powerUpNum = 0;
			}
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) )
		{
			powerUp_->StatusChenge();

			isPowerUp = false;
		}
		powerUp_->SetSlect(powerUpNum);
	}
	else
	{
		player_->Update();
		enemys_->Update();

		CollisionManager::GetInstance()->Update();
	}

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
	if ( isPowerUp )
	{
		powerUp_->Draw();
	}
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
