#include "GameScene.h"

#include"imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"
#include"Input.h"
#include"PlayerBulletManager.h"
#include"GameConfig.h"
#include "FontManager.h"
#include<SceneManager.h>

void GameScene::Initialize()
{
	FontManager::CreateFontHandle(NULL,16,3,"normal");

	EnemyManager::TexLoad();

	player_ = std::make_unique<Player>();
	player_->Initialize();

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Initialize();
	mapChip_->SetPlayer(player_.get());

	powerUp_ = std::make_unique<PowerUpCave>();
	powerUp_->Initialize();
	powerUp_->SetPlayer(player_.get());

	dealer_ = std::make_unique<DealDaemon>();
	dealer_->SetPlayer(player_.get());
	dealer_->Initialize();


	enemys_ = std::make_unique<EnemyManager>();
	enemys_->Initialize();
	enemys_->SetMapChip(mapChip_.get());
	enemys_->SetPlayerPtr(player_.get());
	enemys_->SoundLoad();

	nodeManager_ = NodeManager::GetInstance();
	nodeManager_->SetMapChip(mapChip_.get());
	nodeManager_->SetPlayer(player_.get());
	nodeManager_->SetPowerUp(powerUp_.get());
	nodeManager_->SetDealer(dealer_.get());
	nodeManager_->SetEnemys(enemys_.get());
	nodeManager_->Initialize();
	nodeManager_->SetScrollStop(&scrollStop);
	nodeManager_->StartNodeSet(0);
	backGround_ = LoadGraph(std::string("Resources/BackGround/BackGround.png"));

	bgm_ = LoadSoundMem(std::string("Resources\\Sound\\BGM_gameplay.mp3"));
	PlaySoundMem(bgm_,DX_PLAYTYPE_LOOP);
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	if ( Input::Instance()->TriggerKey(KEY_INPUT_R) )
	{
		nodeManager_->Reset();
	}

	if ( player_->IsPowerUp() )
	{
		powerUp_->Update();

		uint32_t powerUpNum = player_->PowerUp();

		powerUp_->SetSlect(powerUpNum);
		if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) || Input::Instance()->TriggerPadKey(PAD_INPUT_1) )
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

		if ( Input::Instance()->TriggerKey(KEY_INPUT_RETURN) || Input::Instance()->TriggerPadKey(PAD_INPUT_2) )
		{
			dealer_->Deal();
		}

		if ( Input::Instance()->TriggerKey(KEY_INPUT_SPACE) || Input::Instance()->TriggerPadKey(PAD_INPUT_1) )
		{
			dealer_->PartsChenge();

			player_->EndChangeParts();
		}
	}
	else
	{
		nodeManager_->Update();

		if ( !nodeManager_->IsMapDraw() )
		{
			player_->Update();
		}

		if ( nodeManager_->IsNodeReset() )
		{
			scrollStop = false;
		}

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
			enemys_->EnemysClear();
		}

		if ( nodeManager_->GameEnd() )
		{
			SceneManager::GetInstance()->ChangeScene("CLEAR");
			enemys_->EnemysClear();
		}
	}

	Vector2 s = Scroll();

	ImGui::Begin("Scroll");

	ImGui::Text("%f,%f",s.x,s.y);

	ImGui::End();
}

void GameScene::Draw()
{
	Vector2 s = Scroll();
	enemys_->SetScroll(s);
	DrawGraph(0,0,backGround_,true);
	mapChip_->Draw(s);
	nodeManager_->Draw();
	player_->Draw(s);
	nodeManager_->MapDraw();
	//if (!chenged) powerUp_->Draw();


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
	EnemyManager::Finalize();
	FontManager::Finalize();
	StopSoundMem(bgm_);
}

Vector2 GameScene::Scroll()
{
	if (scrollStop )
	{
		return nowScroll;
	}
	else
	{
		Vector2 scroll = { 0,0 };

		Vector2 playerPos = player_->GetPos();

		Vector2 mapChipLeftTopPos = { 0,0 };
		Vector2 mapChipTopBottomPos = mapChip_->GetRightTopBottom();

		int32_t WindowHeight = GameConfig::GetWindowHeight();
		int32_t WindowWidth = GameConfig::GetWindowWidth();

		scroll.x = WindowWidth / 2 - playerPos.x;

		scroll.y = WindowHeight / 2 - playerPos.y;

		if ( mapChipTopBottomPos.x > 0 && mapChipTopBottomPos.y > 0 )
		{
			if ( playerPos.x >= mapChipTopBottomPos.x - WindowWidth / 2 )
			{
				scroll.x = -( mapChipTopBottomPos.x - WindowWidth );
				scrollStop = true;
			}
			if ( playerPos.y >= mapChipTopBottomPos.y - WindowHeight / 2 )
			{
				scroll.y = -( mapChipTopBottomPos.y - WindowHeight );
			}
		}
		if ( playerPos.x <= mapChipLeftTopPos.x + WindowWidth / 2 )
		{
			scroll.x = mapChipLeftTopPos.x;
		}
		if ( playerPos.y <= mapChipLeftTopPos.y + WindowHeight / 2 )
		{
			scroll.y = mapChipLeftTopPos.y;
		}

		nowScroll = scroll;

		return scroll;
	}
}
