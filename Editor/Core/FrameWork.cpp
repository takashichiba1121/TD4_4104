#include "FrameWork.h"

void FrameWork::Initialize()
{
	// DxLib初期化
	dxlibManager_ = std::make_unique<DxlibManager>();
	dxlibManager_->Initialize();

	imGuiManager_->Initialize();

	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->SetScreenGraph(dxlibManager_->GetScreen());
}

void FrameWork::Finalize()
{
	imGuiManager_->Finalize();
	dxlibManager_->Finalize();
	sceneManager_->Finalize();
}

void FrameWork::Update()
{
	sceneManager_->Update();
}

void FrameWork::PostUpdate()
{
}

void FrameWork::EditorDraw()
{
	dxlibManager_->ClearScreen();
	sceneManager_->Draw();
	dxlibManager_->ShotScreen();
	dxlibManager_->ClearScreen();

	sceneManager_->ProcessingDraw();
	dxlibManager_->ShotScreen();
	dxlibManager_->ClearScreen();
	dxlibManager_->RefreshDirect3D();
}

bool FrameWork::UserEndRequst()
{
	return false;
}

void FrameWork::UIDraw()
{
	sceneManager_->UIDraw();
	dxlibManager_->RefreshDirect3D();

	imGuiManager_->Draw();
	dxlibManager_->RefreshDirect3D();

	dxlibManager_->FrameEnd();
}

void FrameWork::UIUpdate()
{
	imGuiManager_->Bigin();

	sceneManager_->UIUpdate();

	imGuiManager_->End();
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

		EditorDraw();

		UIUpdate();

		UIDraw();

		PostUpdate();
	}

	Finalize();
}
