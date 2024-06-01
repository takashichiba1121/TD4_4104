#include "GameScene.h"

#include"imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"
#include"Input.h"

#include<DxLib.h>



void GameScene::Initialize()
{

	CollisionManager::GetInstance()->SetMapChip(testMap);

	player_ = std::make_unique<Player>();
	player_->Initialze();
	testenemy_ = std::make_unique<WalkEnemy>();
	testenemy_->Initialize();
	testenemy2_ = std::make_unique<WalkEnemy>();
	testenemy2_->Initialize();
	testenemy2_->SetPos({ 300,100 });
}

void GameScene::Update()
{
	//ImGui::ShowDemoWindow();

	CollisionManager::GetInstance()->Update();

	player_->Update();
	testenemy_->Update();
	testenemy2_->Update();
}

void GameScene::Draw()
{
	for ( size_t i = 0; i < testMap.size(); i++ )
	{
		for ( size_t j = 0; j < testMap[ i ].size(); j++ )
		{
			if ( testMap[ i ][ j ] )
			{
				DrawBox(16 + j * 32 - 16,16 + i * 32 - 16,16 + j * 32 + 16,16 + i * 32 + 16,GetColor(255,255,255),true);
			}
		}
	}
	player_->Draw();
	testenemy_->Draw();
	testenemy2_->Draw();

}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
