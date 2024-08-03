#pragma once
#include"GameConfig.h"
class DxlibManager
{
private:
	
public:


	bool Initialize(const GameConfig::Config* config);

	void Finalize();

	void ClearScreen();

	void RefreshDirect3D();

	void FrameEnd();

	bool EndRequst();

};

