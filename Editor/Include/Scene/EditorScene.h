#pragma once
#include "BaseScene.h"

#include<d3d11.h>
#include<Vec2.h>

#include<vector>
#include<list>
#include<imgui.h>

#include"ChipIndex.h"
#include"Graph.h"

class EditorScene : public BaseScene
{
private:

	struct TableElement
	{
		int8_t id;
		ChipIndex index;
	};




private:
	const int32_t BAR_SIZE = 16;
	const int2 VIEW_WINDOW_SIZE = { 854 + BAR_SIZE,480 + BAR_SIZE };
	const int2 SELECT_VIEW_WINDOW_SIZE = { 185 + BAR_SIZE,185 + BAR_SIZE };
	const int2 SELECT_WINDOW_SIZE = { 185 + BAR_SIZE,279 + BAR_SIZE };
	const int2 VIEW_WINDOW_SIZE_HALF = { VIEW_WINDOW_SIZE.x / 2,VIEW_WINDOW_SIZE.y / 2 };
	const int2 WINDOW_SIZE = {1280,720};
	const  float2 INIT_SCALE = { 1280.0f / 854.0f, 720.0f / 480.0f };

	std::vector<std::vector<int8_t>>editorMap_;
	std::list<TableElement>chips_ = { {ChipIndex::NONE,ChipIndex::NONE },{ChipIndex::ROAD,ChipIndex::ROAD},{ChipIndex::ROOM,ChipIndex::ROOM},{ChipIndex::LOCK_ROOM,ChipIndex::LOCK_ROOM} };
	ImVector<int8_t>tableSelection;
	Graph screenGraph_;
	int32_t screen_;
	int2 mapBlockSize_ = { 10 ,10 };
	int2 tmpBlockSize = mapBlockSize_;
	float blockSize_ = 32;
	float blockSizeHalf_ = blockSize_/2;
	float2 scale_ = { 1280.0f / 854.0f,  720.0f / 480.0f };
	float2 editorViewCenter;
	int2 editorMapSize;
	
	int2 mapCenter_ = { 0,0 };
	int2 screenPos_ = { 0,0 };

	int2 screenMousePos_ = { 0,0 };
	int2 screenOldMousePos_ = { 0,0 };
	int2 editorMousePos_ = { 0,0 };

	float2 scaleUV1_ = {0,0};
	float2 scaleUV2_ = { 1,1 };

	float2 moveUV_ = { 0,0 };

	UV screenUV;
	UV oldScreenUV;

	int32_t mouseInput_;

	Graph noneGraphHandle_;
	Graph roadGraphHandle_;
	Graph doorGraphHandle_;
	Graph roomGraphHandle_;
	Graph lockroomGraphHandle_;

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
	void SelectView();
	void MenuView();
	void EditorMove();
	void EditorScale();
	int2 GetEditorMousePos();
	void ChipDraw(size_t x,size_t y,int8_t chip,int32_t sign = -1);
	void SelectDraw(ChipIndex chip);

	void New();
	
	bool IsEditorMapWithin(int32_t x,int32_t y);

};

