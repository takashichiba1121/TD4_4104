#include "FrameWork.h"

void FrameWork::Initialize()
{
	// DxLib初期化
	dxlibManager_ = std::make_unique<DxlibManager>();
	dxlibManager_->Initialize(GameConfig::GetGameConfig());

	// Imgui初期化
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Initialize();

	//fps初期化
	fps_ = std::make_unique<FPS>();
	fps_->SetFrameRate(GameConfig::GetGameConfig()->fps);

	sceneManager_ = SceneManager::GetInstance();
}

void FrameWork::Finalize()
{
	imguiManager_->Finalize();
	dxlibManager_->Finalize();
	sceneManager_->Finalize();
}

void FrameWork::Update()
{
	fps_->FpsControlBegin();

	imguiManager_->Bigin();

	sceneManager_->Update();

	imguiManager_->End();
}

void FrameWork::PostUpdate()
{
	fps_->FpsControlEnd();
}

void FrameWork::Draw()
{
	dxlibManager_->ClearScreen();

	sceneManager_->Draw();

	dxlibManager_->RefreshDirect3D();

	imguiManager_->Draw();

	dxlibManager_->RefreshDirect3D();

	dxlibManager_->FrameEnd();
}

bool FrameWork::UserEndRequst()
{
	return false;
}

bool FrameWork::EndRequst()
{
	return dxlibManager_->EndRequst() || UserEndRequst();
}

void FrameWork::Run()
{
	Initialize();

	// メインループ
	while ( true )
	{
		if ( EndRequst() )
		{
			break;
		}

		Update();

		Draw();

		PostUpdate();
	}

	Finalize();
}
