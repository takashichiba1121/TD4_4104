#include "EditorScene.h"

#include"DxlibInclude.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "magic_enum.hpp"


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

	editorMapSize = { mapBlockSize_.x * ( int ) blockSize_ ,mapBlockSize_.y * ( int ) blockSize_ };

	noneGraphHandle_ = Load("Resource/NoneChip.png");
	roadGraphHandle_ = Load("Resource/RoadChip.png");
	doorGraphHandle_ = Load("Resource/DoorChip.png");
	roomGraphHandle_ = Load("Resource/RoomChip.png");
	lockroomGraphHandle_ = Load("Resource/LockroomChip.png");
}

void EditorScene::Update()
{
	screenOldMousePos_ = screenMousePos_;
	mouseInput_ = GetMouseInput();

	GetMousePoint(&screenMousePos_.x,&screenMousePos_.y);

	oldScreenUV = screenUV;

	EditorScale();

	EditorMove();

	editorMousePos_ = GetEditorMousePos();

	//if ( IsEditorMapWithin(editorMousePos_.x / GetBlockSize(),editorMousePos_.y / GetBlockSize()) )
	//{
	//	if ( mouseInput_ )
	//	{
	//		editorMap_[ editorMousePos_.y / GetBlockSize() ][ editorMousePos_.x / GetBlockSize() ] = selectChip_;
	//	}
	//}

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

			ChipDraw(centerX,centerY,editorMap_[ i ][ j ]);
			DrawBoxAA(( centerX - blockSizeHalf ),( centerY - blockSizeHalf ),( centerX + blockSizeHalf ),( centerY + blockSizeHalf ),0xffffffff,false);
		}
	}

	//ChipDraw(( editorMousePos_.x / GetBlockSize() ) * 32,( editorMousePos_.y / GetBlockSize() ) * 32,selectChip_,0);

	SetDrawScreen(DX_SCREEN_BACK);
}

void EditorScene::UIUpdate()
{
	EditorView();

	//SelectView();

	//MenuView();

}


void EditorScene::UIDraw()
{
	DrawFormatString(0,550,255,"%d,%d",editorMousePos_.x,editorMousePos_.y);
	//DrawFormatString(0,570,255,"%d,%d,%d",editorMousePos_.x / GetBlockSize(),editorMousePos_.y / GetBlockSize(),( int ) GetBlockSize());

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
			( ImGui::GetWindowSize().x - ( mapBlockSize_.x * blockSize_ ) ) * 0.5f ,
			( ImGui::GetWindowSize().y - ( mapBlockSize_.y * blockSize_ ) ) * 0.5f
	};

	ImGui::SetCursorPos(editorViewCenter);

	ImGui::Image(screenGraph_.pSRV,{ mapBlockSize_.x * blockSize_,mapBlockSize_.y * blockSize_ },screenUV.min,screenUV.max);

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

	std::list<TableElement>::iterator itr = chips_.begin();

	for ( size_t i = 0; i < chips_.size(); i++ )
	{
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		const bool item_is_selected = tableSelection.contains(itr->id);

		ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
		if ( ImGui::Selectable(magic_enum::enum_name(itr->index).data(),item_is_selected,selectable_flags) )
		{
			if ( ImGui::GetIO().KeyCtrl )
			{
				if ( item_is_selected )
					tableSelection.find_erase_unsorted(itr->id);
				else
					tableSelection.push_back(itr->id);
			}
			else
			{
				tableSelection.clear();
				tableSelection.push_back(itr->id);

				selectChip_ = ( ChipIndex ) tableSelection[ 0 ];
			}
		}

		itr++;
	}

	ImGui::EndTable();

	ImGui::End();
}

void EditorScene::MenuView()
{
	ImGui::Begin("MenuView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) SELECT_VIEW_WINDOW_SIZE.x,( float ) SELECT_VIEW_WINDOW_SIZE.y + SELECT_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x + SELECT_VIEW_WINDOW_SIZE.x ,0 });

	ImGui::InputInt2("MapSize",tmpBlockSize.data);
	if ( ImGui::Button("New") )
	{
		New();
	}

	ImGui::Button("Reset");
	ImGui::Button("Export");
	ImGui::Button("Load");
	ImGui::Button("ChangeMode");

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

int2 EditorScene::GetEditorMousePos()
{
	int2 ret{};
	float2 rate;
	float2 sca = GetScale();

	int2 mapHalfSize =
	{
		 blockSize_ * float(mapBlockSize_.x) / 2 ,
		 blockSize_ * float(mapBlockSize_.y) / 2
	};

	int2 editorScreenPos =
	{
		VIEW_WINDOW_SIZE_HALF.x - mapHalfSize.x ,
		VIEW_WINDOW_SIZE_HALF.y - mapHalfSize.x
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

	rate.x /= mapBlockSize_.x * blockSize_;
	rate.y /= mapBlockSize_.y * blockSize_;

	rate.y = Lerp(minUvScreenPos.y,maxUvScreenPos.y,rate.y);
	rate.x = Lerp(minUvScreenPos.x,maxUvScreenPos.x,rate.x);

	ret.x = int(rate.x / blockSize_);
	ret.y = int(rate.y / blockSize_);

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
	case DOOR:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),doorGraphHandle_.handle,true);
		break;
	case ROOM:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),roomGraphHandle_.handle,true);
		break;
	case LOCK_ROOM:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),lockroomGraphHandle_.handle,true);
		break;
	default:
		break;
	}
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

	DeleteGraph(screenGraph_.handle);
	screenGraph_.pSRV->Release();

	SetDrawValidGraphCreateFlag(TRUE);
	screenGraph_.handle = MakeGraph(mapBlockSize_.x * blockSize_,mapBlockSize_.y * blockSize_);
	screenGraph_.pSRV = GetImageResource11(screenGraph_.handle);
	SetDrawValidGraphCreateFlag(FALSE);
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
	case DOOR:
		ImGui::Image(doorGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case ROOM:
		ImGui::Image(roomGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
	case LOCK_ROOM:
		ImGui::Image(lockroomGraphHandle_.pSRV,{ blockSize_ * 5.8f,blockSize_ * 5.8f });
		break;
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
