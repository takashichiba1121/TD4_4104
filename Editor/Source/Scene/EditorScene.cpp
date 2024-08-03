#include "EditorScene.h"

#include"DxlibInclude.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "magic_enum.hpp"
#include<fstream>
#include<sstream>


void EditorScene::Initialize(int32_t screen)
{
	SetDrawValidGraphCreateFlag(TRUE);
	screenGraph_.handle = MakeGraph(mapBlockSize_.x * blockSize_,mapBlockSize_.y * blockSize_);
	screenGraph_.pSRV = GetImageResource11(screenGraph_.handle);
	SetDrawValidGraphCreateFlag(FALSE);

	screen_ = screen;

	//変える予定
	mapCenter_.x = ( mapBlockSize_.x * blockSize_ ) / 2;
	mapCenter_.y = ( mapBlockSize_.y * blockSize_ ) / 2;

	editorMap_.resize(mapBlockSize_.y);
	for ( size_t i = 0; i < editorMap_.size(); i++ )
	{
		editorMap_[ i ].resize(mapBlockSize_.x);
	}

	if ( mapBlockSize_.x > mapBlockSize_.y )
	{
		float aspectRatio = static_cast< float >( mapBlockSize_.y ) / mapBlockSize_.x;

		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y * aspectRatio;

	}
	else if ( mapBlockSize_.x < mapBlockSize_.y )
	{
		float aspectRatio = static_cast< float >( mapBlockSize_.x ) / mapBlockSize_.y;

		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y * aspectRatio;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y;
	}
	else
	{
		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y;
	}

	noneGraphHandle_ = Load("Resource/NoneChip.png");

	roadGraphHandle_ = Load("Resource/RoadChip.png");
	wallGraphHandle_ = Load("Resource/WallChip.png");

	startGraphHandle_ = Load("Resource/Start.png");
	nextGraphHandle_ = Load("Resource/Next.png");

	shortRangeEnemyGraphHandle_ = Load("Resource/ShortRangeEnemy.png");
	longRangeEnemyGraphHandle_ = Load("Resource/LongRangeEnemy.png");
	flyRangeEnemyGraphHandle_ = Load("Resource/FlyRangeEnemy.png");
	randomEnemyGraphHandle_ = Load("Resource/RandomEnemy.png");


	{
		auto files = GetFilesName("Export/Room");

		for ( auto& file : files )
		{
			Rooms room;
			room.name = file;

			auto roomFiles = GetFilesName("Export/Room/" + file);
			for ( auto& roomFile : roomFiles )
			{
				FileTableElement element;
				element.value = roomFile;
				element.id = RandId();
				element.filePath = "Export/Room/" + file + '/' + roomFile + ".json";
				element.directoryPath = "Export/Room/" + file;
				room.roomFiles.push_back(element);
			}

			GenerateRoomExample(room.name,room.example);

			FileTableElement element = { RandId(),{"example"} };
			element.directoryPath = "Export/Room/" + file;
			room.roomFiles.push_back(element);

			rooms_.push_back(room);
		}
	}

	{
		std::list<std::string> tmp = GetFilesName("Export/Map");

		mapFileName_.clear();
		for ( std::string name : tmp )
		{
			FileTableElement element;
			element.value = name;
			element.id = RandId();
			element.filePath = "Export/Map/" + name + ".json";
			element.directoryPath = "Export/Map";
			mapFileName_.push_back(element);
		}
	}

	selectFile_ = &mapFileName_.back();
}

void EditorScene::Update()
{
	screenOldMousePos_ = screenMousePos_;
	mouseInput_ = GetMouseInput();

	GetMousePoint(&screenMousePos_.x,&screenMousePos_.y);

	oldScreenUV = screenUV;

	EditorScale();

	EditorMove();

	editorMousePos_ = GetEditorMousePos(&mouseUvPos_);

	if ( IsEditorMapWithin(editorMousePos_.x,editorMousePos_.y) )
	{
		if ( mouseInput_ & MOUSE_INPUT_LEFT)
		{
			editorMap_[ editorMousePos_.y ][ editorMousePos_.x ].chip = selectChip_;
		}

		if ( mouseInput_ & MOUSE_INPUT_RIGHT )
		{
			editorMap_[ editorMousePos_.y ][ editorMousePos_.x ].chip = NONE;
		}
	}

}

void EditorScene::Draw()
{
	SetDrawScreen(screenGraph_.handle);
	ClearDrawScreen();

	int32_t blockSizeHalf = blockSize_ / 2;

	for ( size_t i = 0; i < mapBlockSize_.y; i++ )
	{
		for ( size_t j = 0; j < mapBlockSize_.x; j++ )
		{
			int32_t centerX = blockSizeHalf + blockSize_ * j;
			int32_t centerY = blockSizeHalf + blockSize_ * i;

			ChipDraw(centerX,centerY,editorMap_[ i ][ j ].chip);
			DrawBoxAA(( centerX - blockSizeHalf ),( centerY - blockSizeHalf ),( centerX + blockSizeHalf ),( centerY + blockSizeHalf ),0xffffffff,false);
		}
	}

	{
		int32_t centerX = blockSizeHalf + blockSize_ * editorMousePos_.x;
		int32_t centerY = blockSizeHalf + blockSize_ * editorMousePos_.y;

		ChipDraw(( centerX - blockSizeHalf ),( centerY - blockSizeHalf ),selectChip_,0);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void EditorScene::UIUpdate()
{
	EditorView();

	SelectView();

	MenuView();

	MapSelectView();

	RoomSelectView();

}


void EditorScene::UIDraw()
{
	DrawFormatString(0,550,255,"%d,%d",editorMousePos_.x,editorMousePos_.y);
	DrawFormatString(0,570,255,"%f,%f",mouseUvPos_.x,mouseUvPos_.y);

	DrawFormatString(0,590,255,"%f,%f",scaleUV1_.x,scaleUV1_.y);
	DrawFormatString(0,610,255,"%f,%f",scaleUV2_.x,scaleUV2_.y);
	DrawFormatString(0,630,255,"%f,%f",moveUV_.x,moveUV_.y);

	DrawFormatString(0,650,255,"%f,%f",moveUV_.x + scaleUV1_.x,moveUV_.y + scaleUV1_.y);
	DrawFormatString(0,670,255,"%f,%f",moveUV_.x + scaleUV2_.x,moveUV_.y + scaleUV2_.y);

}

void EditorScene::SpriteDraw()
{
}

void EditorScene::Finalize()
{
}

int EditorScene::GetBlockSize()
{
	return  blockSize_ * ( scale_.x / ( 1280.0f / 854.0f ) );
}

float2 EditorScene::GetScale()
{
	float2 ret =
	{
		 scale_.x / INIT_SCALE.x ,
		 scale_.y / INIT_SCALE.y
	};

	return ret;
}

void EditorScene::EditorView()
{
	ImGui::Begin("EditorView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) VIEW_WINDOW_SIZE.x,( float ) VIEW_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ 0,0 });

	editorViewCenter =
	{
			( ImGui::GetWindowSize().x - editorMapSize.x ) * 0.5f ,
			( ImGui::GetWindowSize().y - editorMapSize.y ) * 0.5f
	};

	ImGui::SetCursorPos(editorViewCenter);

	ImGui::Image(screenGraph_.pSRV,{ ( float ) editorMapSize.x,( float ) editorMapSize.y },screenUV.min,screenUV.max);

	ImGui::End();

}

void EditorScene::SelectView()
{
	ImGui::Begin("SelectView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) SELECT_VIEW_WINDOW_SIZE.x,( float ) SELECT_VIEW_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x,0 });

	SelectDraw(selectChip_);

	ImGui::End();

	ImGui::Begin("Select",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) SELECT_WINDOW_SIZE.x,( float ) SELECT_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x,( float ) SELECT_VIEW_WINDOW_SIZE.y });

	ImGui::BeginTable("table",1,
		ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
		| ImGuiTableFlags_SortMulti
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
		| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerH);


	ImGui::TableHeadersRow();

	std::list<ChipTableElement>::iterator itr = chips_.begin();

	for ( size_t i = 0; i < chips_.size(); i++ )
	{
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		const bool item_is_selected = tableSelection.contains(itr->id);

		ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
		if ( ImGui::Selectable(magic_enum::enum_name(itr->value).data(),item_is_selected,selectable_flags) )
		{
			tableSelection.clear();
			tableSelection.push_back(itr->id);

			selectChip_ = ( ChipIndex ) tableSelection[ 0 ];
		}

		itr++;
	}

	ImGui::EndTable();

	ImGui::End();
}

void EditorScene::MenuView()
{
	ImGui::Begin("MenuView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) SELECT_VIEW_WINDOW_SIZE.x,( float ) SELECT_VIEW_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x + SELECT_VIEW_WINDOW_SIZE.x ,0 });

	if ( mode_ == EditMode::MAP )
	{
		ImGui::InputInt2("MapSize",tmpBlockSize.data);
		if ( ImGui::Button("New") )
		{
			New();
		}
	}

	if ( ImGui::Button("Reset") )
	{
		Reset();
	}

	ImGui::InputText("name",textBuff,_countof(textBuff));

	mapName_.clear();
	mapName_ = textBuff;

	if ( ImGui::Button("Export") )
	{
		if ( !mapName_.empty() )
		{
			Export();
		}
	}

	if ( ImGui::Button("Load") )
	{
		LoadFile();
	}

	ImGui::End();
}

void EditorScene::EditorMove()
{
	if ( CheckHitKey(KEY_INPUT_LSHIFT) && mouseInput_ & MOUSE_INPUT_LEFT )
	{
		int2 mouseMove = screenOldMousePos_ - screenMousePos_;

		moveUV_.x += ( float ) mouseMove.x / 1000;
		moveUV_.y += ( float ) mouseMove.y / 1000;

		screenUV.min = scaleUV1_ + moveUV_;
		screenUV.max = scaleUV2_ + moveUV_;

		if ( screenUV.min.x < 0 || screenUV.max.x>1.0f )
		{
			screenUV.min.x = oldScreenUV.min.x;
			screenUV.max.x = oldScreenUV.max.x;
		}

		if ( screenUV.min.y < 0 || screenUV.max.y>1.0f )
		{
			screenUV.min.y = oldScreenUV.min.y;
			screenUV.max.y = oldScreenUV.max.y;
		}
	}
}

void EditorScene::MapSelectView()
{

	ImGui::Begin("MapSelectView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) MAP_SELECT_WINDOW_SIZE.x,( float ) MAP_SELECT_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x + SELECT_VIEW_WINDOW_SIZE.x ,float(SELECT_VIEW_WINDOW_SIZE.y) });

	ImGui::BeginTable("table2",1,
	ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
	| ImGuiTableFlags_SortMulti
	| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
	| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
	| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerH);


	ImGui::TableHeadersRow();

	for (auto& itr : mapFileName_)
	{
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		const bool item_is_selected = mapsOrRoomsTableSelection.contains(itr.id);

		ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
		if ( ImGui::Selectable(itr.value.data(),item_is_selected,selectable_flags) )
		{
			mapsOrRoomsTableSelection.clear();
			mapsOrRoomsTableSelection.push_back(itr.id);
			selectFile_ = &itr;
			selectMode_ = EditMode::MAP;
		}
	}
	ImGui::EndTable();

	ImGui::End();
}

int32_t EditorScene::RandId()
{
	std::random_device rnd;
	std::mt19937 mt(rnd());

	int32_t ret = mt();
	while ( randNums_.find(ret) != randNums_.end() )
	{
		ret = mt();
	}

	randNums_.insert(ret);

	return ret;
}

void EditorScene::RoomSelectView()
{
	ImGui::Begin("RoomSelectView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) ROOM_SELECT_WINDOW_SIZE.x,( float ) ROOM_SELECT_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x + SELECT_VIEW_WINDOW_SIZE.x ,float(SELECT_VIEW_WINDOW_SIZE.y + MAP_SELECT_WINDOW_SIZE.y) });

	for ( auto& room : rooms_ )
	{
		if ( ImGui::TreeNode(room.name.c_str()) )
		{
			ImGui::BeginTable(( room.name + "table" ).c_str(),1,
			ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
			| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerH);

			ImGui::TableHeadersRow();

			for ( auto& file : room.roomFiles )
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				const bool item_is_selected = mapsOrRoomsTableSelection.contains(file.id);

				ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
				if ( ImGui::Selectable(file.value.data(),item_is_selected,selectable_flags) )
				{
					mapsOrRoomsTableSelection.clear();
					mapsOrRoomsTableSelection.push_back(file.id);
					selectFile_ = &file;
					selectMode_ = EditMode::ROOM;
					selectRooms_ = &room;
				}
			}

			ImGui::EndTable();

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void EditorScene::GenerateRoomExample(const std::string& path,std::vector<std::vector<uint8_t>>& example)
{
	std::vector<std::string>settings = Split(path,'@');

	char w = settings[ 1 ].data()[ 0 ],h = settings[ 1 ].data()[ 2 ];
	int2 size = { atoi(&w),atoi(&h) };
	size.x += 2;
	size.y += 2;
	example.resize(size.y);
	for ( size_t i = 0; i < example.size(); i++ )
	{
		example[ i ].resize(size.x);

		for ( size_t j = 0; j < size.x; j++ )
		{
			if ( i == 0 || i == ( size.y - 1 ) || j == 0 || j == ( size.x - 1 ) )
			{
				example[ i ][ j ] = ChipIndex::WALL;
			}
		}
	}

	std::vector<std::string>rooms = Split(settings[ 2 ],'I');

	std::array<std::string,2> nums;
	int2 roomDoor{};
	for ( auto& room : rooms )
	{
		nums = NumSplit(room);
		roomDoor.x = atoi(nums[ 0 ].data());
		roomDoor.y = atoi(nums[ 1 ].data());

		example[ 1 + roomDoor.y ][ 1 + roomDoor.x ] = ChipIndex::DOOR;

	}

}

void EditorScene::RoomSearch(RoomSetting& roomSetting,int32_t x,int32_t y,nlohmann::ordered_json& jsonData)
{
	const std::array<int2,4>WEYS = { {{-1,0,},{0,-1},{1,0,},{0,1}} };

	enum weys
	{
		RIGHT = 2,
		LEFT = 0,
		DOWN = 3,
		UP = 1
	};

	int2 localPos = { x,y };
	roomSetting.leftTop = localPos;
	weys wey = RIGHT;
	localPos += WEYS[ UP ];
	//localPos += WEYS[ RIGHT ];
	int2 size = {};

	while ( true )
	{
		if ( editorMap_[ localPos.y ][ localPos.x ].chip == ChipIndex::DOOR )
		{
			roomSetting.doors.push_back({ localPos.x - x,localPos.y - y });
		}

		if ( wey == UP )
		{
			if ( editorMap_[ localPos.y ][ localPos.x + 1 ].chip == ChipIndex::LOCK_ROOM && roomSetting.lock ||
				 editorMap_[ localPos.y ][ localPos.x + 1 ].chip == ChipIndex::ROOM && !roomSetting.lock )
			{
				localPos += WEYS[ wey ];
			}
			else
			{
				break;
			}
		}

		switch ( wey )
		{
		case RIGHT:
			if ( editorMap_[ localPos.y + 1 ][ localPos.x ].chip == ChipIndex::LOCK_ROOM && roomSetting.lock ||
				 editorMap_[ localPos.y + 1 ][ localPos.x ].chip == ChipIndex::ROOM && !roomSetting.lock )
			{
				localPos += WEYS[ wey ];
				size.x++;
			}
			else
			{
				wey = DOWN;
				localPos += WEYS[ wey ];
			}

			break;
		case DOWN:

			if ( editorMap_[ localPos.y ][ localPos.x - 1 ].chip == ChipIndex::LOCK_ROOM && roomSetting.lock ||
				 editorMap_[ localPos.y ][ localPos.x - 1 ].chip == ChipIndex::ROOM && !roomSetting.lock )
			{
				localPos += WEYS[ wey ];
				size.y++;
			}
			else
			{
				wey = LEFT;
				localPos += WEYS[ wey ];
			}
			break;
		case LEFT:
			if ( editorMap_[ localPos.y - 1 ][ localPos.x ].chip == ChipIndex::LOCK_ROOM && roomSetting.lock ||
				 editorMap_[ localPos.y - 1 ][ localPos.x ].chip == ChipIndex::ROOM && !roomSetting.lock )
			{
				localPos += WEYS[ wey ];
			}
			else
			{
				wey = UP;
				localPos += WEYS[ wey ];
			}
			break;
		}

		editorMap_[ localPos.y ][ localPos.x ].room = true;
	}

	for ( size_t i = y; i < y + size.y; i++ )
	{
		for ( size_t j = x; j < x + size.x; j++ )
		{
			editorMap_[ i ][ j ].room = true;
		}
	}

	roomSetting.size = size;
}

void EditorScene::EditorScale()
{
	int mouseWheelRotVol = GetMouseWheelRotVol();

	if ( mouseWheelRotVol != 0 )
	{
		scaleUV1_.x += 0.01f * mouseWheelRotVol;
		scaleUV1_.y += 0.01f * mouseWheelRotVol;
		scaleUV2_.x += 0.01f * -mouseWheelRotVol;
		scaleUV2_.y += 0.01f * -mouseWheelRotVol;

		scaleUV1_ = Min(scaleUV1_,0.5f);
		scaleUV2_ = Min(scaleUV2_,1.0f);

		scaleUV1_ = Max(scaleUV1_,0.0001f);
		scaleUV2_ = Max(scaleUV2_,0.5f);

		screenUV.min = scaleUV1_ + moveUV_;
		screenUV.max = scaleUV2_ + moveUV_;

		if ( screenUV.min.x < 0 || screenUV.max.x>1.0f )
		{
			screenUV.min.x = oldScreenUV.min.x;
			screenUV.max.x = oldScreenUV.max.x;
		}

		if ( screenUV.min.y < 0 || screenUV.max.y>1.0f )
		{
			screenUV.min.y = oldScreenUV.min.y;
			screenUV.max.y = oldScreenUV.max.y;
		}
	}
}

int2 EditorScene::GetEditorMousePos(float2* rte)
{
	int2 ret{};
	float2 rate;
	float2 sca = GetScale();

	int2 mapHalfSize =
	{
		 editorMapSize.x / 2 ,
		 editorMapSize.y / 2
	};

	int2 editorScreenPos =
	{
		VIEW_WINDOW_SIZE_HALF.x - mapHalfSize.x ,
		VIEW_WINDOW_SIZE_HALF.y - mapHalfSize.y
	};

	float2 minUvScreenPos =
	{
		mapBlockSize_.x * blockSize_ * screenUV.min.x,
		mapBlockSize_.y * blockSize_ * screenUV.min.y,
	};

	float2 maxUvScreenPos =
	{
		mapBlockSize_.x * blockSize_ * screenUV.max.x,
		mapBlockSize_.y * blockSize_ * screenUV.max.y,
	};

	rate.x = -( editorScreenPos.x - screenMousePos_.x );
	rate.y = -( editorScreenPos.y - screenMousePos_.y );

	rate.x /= editorMapSize.x;
	rate.y /= editorMapSize.y;

	rate.y = Lerp(minUvScreenPos.y,maxUvScreenPos.y,rate.y);
	rate.x = Lerp(minUvScreenPos.x,maxUvScreenPos.x,rate.x);

	ret.x = int(rate.x / blockSize_);
	ret.y = int(rate.y / blockSize_);

	if ( rte )
	{
		*rte = rate;
	}

	return ret;
}

bool EditorScene::IsEditorMapWithin(int32_t x,int32_t y)
{
	return x >= 0 && y >= 0 && x < mapBlockSize_.x && y < mapBlockSize_.y;
}

void EditorScene::ChipDraw(size_t x,size_t y,int8_t chip,int32_t sign)
{
	switch ( chip )
	{
	case NONE:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),noneGraphHandle_.handle,true);
		break;

	case ROAD:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),roadGraphHandle_.handle,true);
		break;
	case WALL:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),wallGraphHandle_.handle,true);
		break;

	case START:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),startGraphHandle_.handle,true);
		break;
	case NEXT:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),nextGraphHandle_.handle,true);
		break;

	case SHORT_RANGE_ENEMY:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),shortRangeEnemyGraphHandle_.handle,true);
		break;
	case LONG_RANGE_ENEMY:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),longRangeEnemyGraphHandle_.handle,true);
		break;
	case FLY_RANGE_ENEMY:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),flyRangeEnemyGraphHandle_.handle,true);
		break;
	case RANDOM_ENEMY:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),randomEnemyGraphHandle_.handle,true);
		break;

	case DOOR:
	case ROOM:
	case LOCK_ROOM:
	default:
		break;
	}
}

void EditorScene::SelectDraw(ChipIndex chip)
{
	switch ( chip )
	{
	case NONE:
		ImGui::Image(noneGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;

	case ROAD:
		ImGui::Image(roadGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case WALL:
		ImGui::Image(wallGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;

	case START:
		ImGui::Image(startGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case NEXT:
		ImGui::Image(nextGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;

	case SHORT_RANGE_ENEMY:
		ImGui::Image(shortRangeEnemyGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case LONG_RANGE_ENEMY:
		ImGui::Image(longRangeEnemyGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case FLY_RANGE_ENEMY:
		ImGui::Image(flyRangeEnemyGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case RANDOM_ENEMY:
		ImGui::Image(randomEnemyGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;

	case DOOR:
	case ROOM:
	case LOCK_ROOM:
	default:
		break;
	}
}

void EditorScene::ProcessingDraw()
{
	DrawRotaGraph3(
		WINDOW_SIZE.x / 2,WINDOW_SIZE.y / 2,
		mapCenter_.x + screenPos_.x,mapCenter_.y + screenPos_.y,
		scale_.x,scale_.y,
		0,screen_,true);
}

void EditorScene::New()
{
	for ( size_t i = 0; i < mapBlockSize_.y; i++ )
	{
		editorMap_[ i ].clear();
	}
	editorMap_.clear();

	mapBlockSize_ = tmpBlockSize;

	mapCenter_.x = ( mapBlockSize_.x * blockSize_ ) / 2;
	mapCenter_.y = ( mapBlockSize_.y * blockSize_ ) / 2;

	editorMap_.resize(mapBlockSize_.y);
	for ( size_t i = 0; i < editorMap_.size(); i++ )
	{
		editorMap_[ i ].resize(mapBlockSize_.x);
	}

	if ( mapBlockSize_.x > mapBlockSize_.y )
	{
		float aspectRatio = static_cast< float >( mapBlockSize_.y ) / mapBlockSize_.x;

		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y * aspectRatio;

	}
	else if ( mapBlockSize_.x < mapBlockSize_.y )
	{
		float aspectRatio = static_cast< float >( mapBlockSize_.x ) / mapBlockSize_.y;

		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y * aspectRatio;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y;
	}
	else
	{
		editorMapSize.x = MAX_EDITOR_MAP_SIZE.y;
		editorMapSize.y = MAX_EDITOR_MAP_SIZE.y;
	}

	DeleteGraph(screenGraph_.handle);
	screenGraph_.pSRV->Release();

	SetDrawValidGraphCreateFlag(TRUE);
	screenGraph_.handle = MakeGraph(mapBlockSize_.x * blockSize_,mapBlockSize_.y * blockSize_);
	screenGraph_.pSRV = GetImageResource11(screenGraph_.handle);
	SetDrawValidGraphCreateFlag(FALSE);
}

void EditorScene::Export()
{
	nlohmann::ordered_json data;

	if ( mode_ == EditMode::MAP )
	{
		data[ "0_Name" ] = mapName_;
		data[ "1_Size" ].push_back({ mapBlockSize_.x, mapBlockSize_.y });

		for ( size_t i = 0; i < mapBlockSize_.y; i++ )
		{
			for ( size_t j = 0; j < mapBlockSize_.x; j++ )
			{
				if ( !editorMap_[ i ][ j ].out )
				{
					data[ "Map" ].push_back(editorMap_[ i ][ j ].chip);

					editorMap_[ i ][ j ].out = true;
				}

			}
		}

		for ( size_t i = 0; i < roomSettings_.size(); i++ )
		{
			nlohmann::ordered_json roomSetting;
			roomSetting[ "Look" ] = roomSettings_[ i ].lock;
			roomSetting[ "LeftTop" ] = { roomSettings_[ i ].leftTop.x,roomSettings_[ i ].leftTop.y };
			roomSetting[ "Size" ] = { roomSettings_[ i ].size.x,roomSettings_[ i ].size.y };
			roomSetting[ "DoorsNum" ] = roomSettings_[ i ].doors.size();

			for ( size_t j = 0; j < roomSettings_[ i ].doors.size(); j++ )
			{
				roomSetting[ "Doors" ].push_back({ roomSettings_[ i ].doors[ j ].x,roomSettings_[ i ].doors[ j ].y });
			}

			std::string roomName;

			if ( roomSettings_[ i ].lock )
			{
				roomName += "Lock@";
			}
			else
			{
				roomName += "NoLock@";
			}

			roomName += std::to_string(roomSettings_[ i ].size.x) + '_' + std::to_string(roomSettings_[ i ].size.y) + '@';

			for ( size_t j = 0; j < roomSettings_[ i ].doors.size() - 1; j++ )
			{
				roomName += std::to_string(roomSettings_[ i ].doors[ j ].x) + '_' + std::to_string(roomSettings_[ i ].doors[ j ].y) + 'I';
			}

			roomName += std::to_string(roomSettings_[ i ].doors.back().x) + '_' + std::to_string(roomSettings_[ i ].doors.back().y);

			std::filesystem::create_directory("Export/Room/" + roomName);
			roomSetting[ "RoomName" ] = roomName;

			{
				auto itr = std::find_if(rooms_.begin(),rooms_.end(),[ & ] (Rooms room)
{
	return room.name == roomName;
});

				if ( itr == rooms_.end() )
				{
					Rooms room;
					room.name = roomName;
					GenerateRoomExample(room.name,room.example);
					FileTableElement element = { RandId(),{"example"} };
					element.directoryPath = "Export/Room/" + roomName;
					room.roomFiles.push_back(element);
					rooms_.push_back(room);
				}
			}

			data[ "3_RoomSettings" ].push_back(roomSetting);
		}

		data[ "2_RoomNum" ].push_back(roomSettings_.size());

		std::ofstream file;
		file.open(selectFile_->directoryPath + '/' + mapName_ + ".json");
		file << data.dump();
		file.close();

		{

			FileTableElement element;
			element.value = mapName_;
			element.id = RandId();
			element.filePath = "Export/Map/" + mapName_ + ".json";
			element.directoryPath = "Export/Map";
			mapFileName_.push_back(element);
		}
	}
	else
	{
		data[ "0_Name" ] = mapName_;
		data[ "1_Size" ].push_back({ mapBlockSize_.x, mapBlockSize_.y });
		for ( size_t i = 0; i < mapBlockSize_.y; i++ )
		{
			for ( size_t j = 0; j < mapBlockSize_.x; j++ )
			{
				if ( !editorMap_[ i ][ j ].out )
				{
					data[ "Room" ].push_back(editorMap_[ i ][ j ].chip);
					editorMap_[ i ][ j ].out = true;
				}

			}
		}
		std::ofstream file;
		file.open(selectFile_->directoryPath + '/' + mapName_ + ".json");
		file << data.dump();
		file.close();

		{
			FileTableElement element;
			element.value = mapName_;
			element.id = RandId();
			element.filePath = selectFile_->directoryPath + '/' + mapName_ + ".json";
			element.directoryPath = selectFile_->directoryPath;
			loadRooms_->roomFiles.push_back(element);
		}
	}

	for ( size_t i = 0; i < mapBlockSize_.y; i++ )
	{
		for ( size_t j = 0; j < mapBlockSize_.x; j++ )
		{
			editorMap_[ i ][ j ].out = false;
			editorMap_[ i ][ j ].room = false;

		}
	}
}

void EditorScene::LoadFile()
{
	loadFile_ = selectFile_;
	mode_ = selectMode_;

	if ( mode_ == EditMode::ROOM )
	{
		loadRooms_ = selectRooms_;
	}
	for ( size_t i = 0; i < mapBlockSize_.y; i++ )
	{
		editorMap_[ i ].clear();
	}
	editorMap_.clear();

	if ( mode_ == EditMode::ROOM && loadFile_->value == "example" )
	{
		editorMap_.resize(loadRooms_->example.size());

		for ( size_t i = 0; i < loadRooms_->example.size(); i++ )
		{
			editorMap_[ i ].resize(loadRooms_->example[ i ].size());

			for ( size_t j = 0; j < loadRooms_->example[ i ].size(); j++ )
			{
				editorMap_[ i ][ j ].chip = loadRooms_->example[ i ][ j ];
			}
		}

		mapBlockSize_ = { int32_t(loadRooms_->example[ 0 ].size()) ,int32_t(loadRooms_->example.size()) };
	}
	else
	{
		std::fstream file;
		file.open(selectFile_->filePath);
		if ( file.fail() )
		{
			assert(0);
		}

		nlohmann::ordered_json jsonData;
		file >> jsonData;
		file.close();
		editorMap_.resize(jsonData[ "1_Size" ][ 0 ][ 1 ]);
		for ( size_t i = 0; i < editorMap_.size(); i++ )
		{
			editorMap_[ i ].resize(jsonData[ "1_Size" ][ 0 ][ 0 ]);
		}

		if ( mode_ == EditMode::ROOM )
		{
			size_t index = 0;
			for ( size_t i = 0; i < editorMap_.size(); i++ )
			{
				for ( size_t j = 0; j < editorMap_[ i ].size(); j++ )
				{
					editorMap_[ i ][ j ].chip = jsonData[ "Room" ][ index ];
					editorMap_[ i ][ j ].out = false;
					index++;
				}
			}
		}
		else
		{
			size_t index = 0;
			for ( size_t i = 0; i < editorMap_.size(); i++ )
			{
				for ( size_t j = 0; j < editorMap_[ i ].size(); j++ )
				{
					editorMap_[ i ][ j ].chip = jsonData[ "Map" ][ index ];
					editorMap_[ i ][ j ].out = false;
					index++;
				}
			}
		}

		mapBlockSize_ = { jsonData[ "1_Size" ][ 0 ][ 0 ] ,jsonData[ "1_Size" ][ 0 ][ 1 ] };
	}

	mapCenter_.x = ( mapBlockSize_.x * blockSize_ ) / 2;
	mapCenter_.y = ( mapBlockSize_.y * blockSize_ ) / 2;

	DeleteGraph(screenGraph_.handle);
	screenGraph_.pSRV->Release();

	SetDrawValidGraphCreateFlag(TRUE);
	screenGraph_.handle = MakeGraph(mapBlockSize_.x * blockSize_,mapBlockSize_.y * blockSize_);
	screenGraph_.pSRV = GetImageResource11(screenGraph_.handle);
	SetDrawValidGraphCreateFlag(FALSE);
}

void EditorScene::Reset()
{
	if ( mode_ == EditMode::MAP )
	{
		for ( size_t i = 0; i < mapBlockSize_.y; i++ )
		{
			for ( size_t j = 0; j < mapBlockSize_.x; j++ )
			{
				editorMap_[ i ][ j ].out = false;
				editorMap_[ i ][ j ].chip = 0;
			}
		}
	}
	else
	{
		for ( size_t i = 0; i < mapBlockSize_.y; i++ )
		{
			for ( size_t j = 0; j < mapBlockSize_.x; j++ )
			{
				editorMap_[ i ][ j ].out = false;
				editorMap_[ i ][ j ].chip = loadRooms_->example[ i ][ j ];

			}
		}
	}
}

std::list<std::string> GetFilesName(const std::string& path)
{
	std::list<std::string>ret;

	auto dir_it = std::filesystem::directory_iterator(path);

	for ( auto& p : dir_it )
	{
		std::string extension = p.path().extension().string();

		if ( extension == ".json" )
		{
			ret.push_back(p.path().filename().stem().string());
		}

	}

	return ret;
}

std::vector<std::string> Split(const std::string& str,char del)
{
	int first = 0;
	int last = str.find_first_of(del);

	std::vector<std::string> result;

	while ( first < str.size() )
	{
		std::string subStr(str,first,last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del,first);

		if ( last == std::string::npos )
		{
			last = str.size();
		}
	}

	return result;
}

std::array<std::string,2> NumSplit(const std::string& str)
{
	int first = 0;
	int last = str.find_first_of('_');

	std::array<std::string,2> result;

	for ( size_t i = 0; i < 2; i++ )
	{
		std::string subStr(str,first,last - first);

		result[ i ] = subStr;

		first = last + 1;
		last = str.find_first_of('_',first);

		if ( last == std::string::npos )
		{
			last = str.size();
		}
	}

	return result;
}
