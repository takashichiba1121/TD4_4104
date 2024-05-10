#pragma once
#include "BaseScene.h"

#include<d3d11.h>
#include<Vec2.h>
#include<vector>

#include"ChipIndex.h"

class EditorScene : public BaseScene
{
private:

	const int2 VIEW_WINDOW_SIZE = { 854 + 16,480 + 16 };
	const int2 VIEW_WINDOW_SIZE_HALF = { VIEW_WINDOW_SIZE.x / 2,VIEW_WINDOW_SIZE.y / 2 };
	const int2 WINDOW_SIZE = {1280,720};
	const  float2 INIT_SCALE = { 1280.0f / 854.0f, 720.0f / 480.0f };
	std::vector<std::vector<int8_t>>editorMap;

	ID3D11ShaderResourceView* pSRV_ = nullptr;
	int32_t screen_;
	int2 mapSize_ = { 10 ,10 };
	float blockSize_ = 32;
	float blockSizeHalf_ = blockSize_/2;
	float2 scale_ = { 1280.0f / 854.0f,  720.0f / 480.0f };
	
	int2 mapCenter_ = { 0,0 };
	int2 screenPos_ = { 0,0 };

	int2 screenMousePos_ = { 0,0 };
	int2 screenOldMousePos_ = { 0,0 };
	int2 editorMousePos_ = { 0,0 };

	int32_t mouseInput_;

	int32_t noneGraphHandle_;
	int32_t roadGraphHandle_;
	int32_t doorGraphHandle_;
	int32_t roomGraphHandle_;
	int32_t lockroomGraphHandle_;

	ChipIndex selectChip_;

public:
	// BaseScene を介して継承されました
	void Initialize(int32_t screen) override;
	void Update() override;
	void UIUpdate() override;
	void Draw() override;
	void ProcessingDraw() override;
	void UIDraw() override;
	void SpriteDraw() override;
	void Finalize() override;
	int GetBlockSize();
	float2 GetScale();
private:

	void EditorView();
	void EditorMove();
	int2 GetEditorMousePos();
	void ChipDraw(size_t x,size_t y,int8_t chip,int32_t sign = -1);

	bool IsEditorMapWithin(int32_t x,int32_t y);

};

