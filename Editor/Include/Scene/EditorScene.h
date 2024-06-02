#pragma once
#include "BaseScene.h"

#include<d3d11.h>
#include<vector>
#include<list>
#include<imgui.h>
#include "json.hpp"

#include<Vec2.h>
#include"ChipIndex.h"
#include"Graph.h"
#include<string>
#include<filesystem>
#include<array>
#include<unordered_set>
#include<random>

std::list<std::string> GetFilesName(const std::string& path);
std::vector<std::string> Split(const std::string& str,char del);
std::array<std::string,2> NumSplit(const std::string& str);

class EditorScene : public BaseScene
{
private:

	enum class EditMode
	{
		ROOM,
		MAP
	};

	template<class T>
	struct TableElement
	{
		int8_t id;
		T value;
		std::string filePath;
		std::string directoryPath;
	};

	using ChipTableElement = TableElement<ChipIndex>;
	using FileTableElement = TableElement<std::string>;

	struct MapIndex
	{
		int8_t chip;
		bool room;
		bool out;
	};

	struct RoomSetting
	{
		bool lock;
		int2 leftTop;
		int2 size;
		std::vector<int2>doors;
	};

	struct Rooms
	{
		std::string name;
		std::list<FileTableElement> roomFiles;
		std::vector<std::vector<uint8_t>>example;

	};

private:
	const int32_t BAR_SIZE = 16;
	const int2 VIEW_WINDOW_SIZE = { 854 + BAR_SIZE,480 + BAR_SIZE };
	const int2 SELECT_VIEW_WINDOW_SIZE = { 185 + BAR_SIZE,185 + BAR_SIZE };
	const int2 SELECT_WINDOW_SIZE = { 185 + BAR_SIZE,279 + BAR_SIZE };
	const int2 MAP_SELECT_WINDOW_SIZE = { 185 + BAR_SIZE,131 + BAR_SIZE };
	const int2 ROOM_SELECT_WINDOW_SIZE = { 185 + BAR_SIZE,132 + BAR_SIZE };

	const int2 VIEW_WINDOW_SIZE_HALF = { VIEW_WINDOW_SIZE.x / 2,VIEW_WINDOW_SIZE.y / 2 };
	const int2 WINDOW_SIZE = { 1280,720 };
	const  float2 INIT_SCALE = { 1280.0f / 854.0f, 720.0f / 480.0f };
	const float2 MAX_EDITOR_MAP_SIZE = { 800.0f ,550.0f};

	std::unordered_set<int32_t>randNums_;

	std::vector<std::vector<MapIndex>>editorMap_;
	std::vector<RoomSetting>roomSettings_;
	std::vector<Rooms>rooms_;
	std::list<ChipTableElement>chips_ =
	{
		{ChipIndex::NONE,ChipIndex::NONE},
		{ChipIndex::ROAD,ChipIndex::ROAD},
		{ChipIndex::DOOR,ChipIndex::DOOR},
		{ChipIndex::ROOM,ChipIndex::ROOM},
		{ChipIndex::LOCK_ROOM,ChipIndex::LOCK_ROOM},
		{ChipIndex::WALL,ChipIndex::WALL} };
	std::list<FileTableElement>mapFileName_;
	std::list<FileTableElement>roomFileName_;
	EditMode selectMode_;
	EditMode mode_= EditMode::MAP;
	FileTableElement selectFile_;
	FileTableElement loadFile_;
	Rooms selectRooms_;
	Rooms loadRooms_;

	ImVector<int8_t>tableSelection;
	ImVector<int32_t>mapsOrRoomsTableSelection;
	char textBuff[ 256 ];
	std::string mapName_;
	Graph screenGraph_;
	int32_t screen_;
	int2 mapBlockSize_ = { 10 ,10 };
	int2 tmpBlockSize = mapBlockSize_;
	float blockSize_ = 32;
	float blockSizeHalf_ = blockSize_ / 2;
	float2 scale_ = { 1280.0f / 854.0f,  720.0f / 480.0f };
	float2 editorViewCenter;
	int2 editorMapSize;

	int2 mapCenter_ = { 0,0 };
	int2 screenPos_ = { 0,0 };

	int2 screenMousePos_ = { 0,0 };
	int2 screenOldMousePos_ = { 0,0 };
	int2 editorMousePos_ = { 0,0 };

	float2 scaleUV1_ = { 0.0001f,0.0001f };
	float2 scaleUV2_ = { 1.0f,1.0f };

	float2 moveUV_ = { 0,0 };

	UV screenUV = { { 0.0001f,0.0001f },{ 1.0f,1.0f } };
	UV oldScreenUV;

	int32_t mouseInput_;

	Graph noneGraphHandle_;
	Graph roadGraphHandle_;
	Graph doorGraphHandle_;
	Graph roomGraphHandle_;
	Graph lockroomGraphHandle_;
	Graph wallGraphHandle_;

	ChipIndex selectChip_;

	float2 mouseUvPos_;

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
	void MapSelectView();
	void RoomSelectView();
	void EditorMove();
	void EditorScale();
	int2 GetEditorMousePos(float2* rte =nullptr);
	void ChipDraw(size_t x,size_t y,int8_t chip,int32_t sign = -1);
	void SelectDraw(ChipIndex chip);
	void GenerateRoomExample(const std::string& path,std::vector<std::vector<uint8_t>>&example);

	void New();
	void Export();
	void LoadFile();
	void Reset();


	bool IsEditorMapWithin(int32_t x,int32_t y);

	void RoomSearch(RoomSetting& roomSetting,int32_t x,int32_t y,nlohmann::ordered_json& jsonData);

	int32_t RandId();
};

