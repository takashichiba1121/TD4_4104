#include<memory>
#include<Windows.h>

#include"GameMain.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,int nCmdShow)
{
	std::make_unique<GameMain>()->Run();

	return 0;
}