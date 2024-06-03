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

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Initialize();
	mapChip_->MapLoad("Resources/Export/Map/t.json");
}

void GameScene::Update()
{
	ImGui::ShowDemoWindow();

	CollisionManager::GetInstance()->Update();

	player_->Update();
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

	mapChip_->Draw({0,0});
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
