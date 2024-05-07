#include "DxlibManager.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"DxlibInclude.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam);

	return 0;
}

bool DxlibManager::Initialize()
{
	DxLib::SetOutApplicationLogValidFlag(false);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowSizeChangeEnableFlag(FALSE,FALSE);
	DxLib::SetMainWindowText("Editor");
	DxLib::SetGraphMode(1280,720,32);
	DxLib::SetWindowSizeExtendRate(1.0);
	DxLib::SetBackgroundColor(0,0,0);
	DxLib::SetUseDirect3DVersion(DX_DIRECT3D_11);
	DxLib::SetHookWinProc(WndProc);
	DxLib::SetAlwaysRunFlag(true);
	if ( DxLib::DxLib_Init() == -1 )
	{
		return false;
	}

	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	screen_ = MakeGraph(1280,720);

	return true;
}

void DxlibManager::Finalize()
{
	DxLib::DxLib_End();
}

void DxlibManager::ClearScreen()
{
	DxLib::ClsDrawScreen();
}

void DxlibManager::RefreshDirect3D()
{
	DxLib::RefreshDxLibDirect3DSetting();
}

void DxlibManager::FrameEnd()
{
	DxLib::ScreenFlip();
}

bool DxlibManager::EndRequst()
{
	if ( ProcessMessage() == -1 )
	{
		return true;
	}

	return false;
}

void DxlibManager::ShotScreen()
{
	GetDrawScreenGraph(0,0,1280,720,screen_);
}

int DxlibManager::GetScreen() const
{
	return screen_;
}
