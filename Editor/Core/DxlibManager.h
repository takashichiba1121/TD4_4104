#pragma once

class DxlibManager
{
private:
	int screen_ =-1 ;
public:

	bool Initialize();

	void Finalize();

	void ClearScreen();

	void RefreshDirect3D();

	void FrameEnd();

	bool EndRequst();

	void ShotScreen();

	int GetScreen() const;
};

