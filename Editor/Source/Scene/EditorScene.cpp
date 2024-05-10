#include "EditorScene.h"

#include"DxlibInclude.h"
#include"DxLib.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "magic_enum.hpp"


void EditorScene::Initialize(int32_t screen)
{

	pSRV_ = GetImageResource11(screen);
	screen_ = screen;

	//変える予定
	mapCenter_.x = ( mapSize_.x * blockSize_ ) / 2;
	mapCenter_.y = ( mapSize_.y * blockSize_ ) / 2;

	editorMap_.resize(mapSize_.y);
	for ( size_t i = 0; i < editorMap_.size(); i++ )
	{
		editorMap_[ i ].resize(mapSize_.x);
	}

	noneGraphHandle_ = LoadGraph("Resource/NoneChip.png");
	roadGraphHandle_ = LoadGraph("Resource/RoadChip.png");
	doorGraphHandle_ = LoadGraph("Resource/DoorChip.png");
	roomGraphHandle_ = LoadGraph("Resource/RoomChip.png");
	lockroomGraphHandle_ = LoadGraph("Resource/LockroomChip.png");
}

void EditorScene::Update()
{
	screenOldMousePos_ = screenMousePos_;
	mouseInput_ = GetMouseInput();

	int mouseWheelRotVol = GetMouseWheelRotVol();

	scale_.x += 0.1f * mouseWheelRotVol;
	scale_.y += 0.1f * mouseWheelRotVol;

	max(scale_.x,scale_.x,0.0000001f);
	max(scale_.y,scale_.y,0.0000001f);

	GetMousePoint(&screenMousePos_.x,&screenMousePos_.y);

	EditorMove();

	editorMousePos_ = GetEditorMousePos();

	selectChip_ = ChipIndex::ROAD;

	if ( IsEditorMapWithin(editorMousePos_.x / GetBlockSize(),editorMousePos_.y / GetBlockSize()) )
	{
		if ( mouseInput_  )
		{
			editorMap_[ editorMousePos_.y / GetBlockSize() ][ editorMousePos_.x / GetBlockSize() ] = selectChip_;
		}
	}

}

void EditorScene::Draw()
{
	int32_t blockSizeHalf = blockSize_ / 2;

	for ( size_t i = 0; i < mapSize_.y; i++ )
	{
		for ( size_t j = 0; j < mapSize_.x; j++ )
		{
			int32_t centerX = blockSizeHalf + blockSize_ * j;
			int32_t centerY = blockSizeHalf + blockSize_ * i;

			ChipDraw(centerX,centerY,editorMap_[ i ][ j ]);
			DrawBoxAA(( centerX - blockSizeHalf ),( centerY - blockSizeHalf ),( centerX + blockSizeHalf ),( centerY + blockSizeHalf ),0xffffffff,false);
		}
	}

	ChipDraw(( editorMousePos_.x / GetBlockSize() ) * 32,( editorMousePos_.y / GetBlockSize() ) * 32,selectChip_,0);

}

void EditorScene::UIUpdate()
{
	EditorView();

	SelectView();

}


void EditorScene::UIDraw()
{
	DrawFormatString(0,650,255,"%d,%d",editorMousePos_.x,editorMousePos_.y);
	DrawFormatString(0,670,255,"%d,%d,%d",editorMousePos_.x / GetBlockSize(),editorMousePos_.y / GetBlockSize(),( int ) GetBlockSize());
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

	ImGui::Image(pSRV_,{ ( float ) VIEW_WINDOW_SIZE.x,( float ) VIEW_WINDOW_SIZE.y });

	ImGui::End();

}

void EditorScene::SelectView()
{
	ImGui::Begin("SelectView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetWindowSize({ ( float ) SELECT_VIEW_WINDOW_SIZE.x,( float ) SELECT_VIEW_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ ( float ) VIEW_WINDOW_SIZE.x,0 });

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
			}
		}

		itr++;
	}

	ImGui::EndTable();

	ImGui::End();


}

void EditorScene::EditorMove()
{
	if ( CheckHitKey(KEY_INPUT_LSHIFT) && mouseInput_ & MOUSE_INPUT_LEFT )
	{
		int2 mouseMove = screenOldMousePos_ - screenMousePos_;

		screenPos_ += mouseMove;
	}
}

int2 EditorScene::GetEditorMousePos()
{
	int2 ret{};

	float2 sca = GetScale();

	int2 mapHalfSize =
	{
		blockSize_ * mapSize_.x / 2 * sca.x ,
		blockSize_ * mapSize_.y / 2 * sca.y
	};

	int2 editorScreenPos =
	{
		VIEW_WINDOW_SIZE_HALF.x - mapHalfSize.x - screenPos_.x ,
		VIEW_WINDOW_SIZE_HALF.y - mapHalfSize.x - screenPos_.y
	};

	ret.x = -( int ) ( editorScreenPos.x - screenMousePos_.x );
	ret.y = -( int ) ( editorScreenPos.y - screenMousePos_.y );
	return ret;
}

bool EditorScene::IsEditorMapWithin(int32_t x,int32_t y)
{
	return x >= 0 && y >= 0 && x < mapSize_.x && y < mapSize_.y;
}

void EditorScene::ChipDraw(size_t x,size_t y,int8_t chip,int32_t sign)
{
	switch ( chip )
	{
	case NONE:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),noneGraphHandle_,true);
		break;
	case ROAD:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),roadGraphHandle_,true);
		break;
	case DOOR:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),doorGraphHandle_,true);
		break;
	case ROOM:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),roomGraphHandle_,true);
		break;
	case LOCK_ROOM:
		DrawGraph(x + ( blockSizeHalf_ * sign ),y + ( blockSizeHalf_ * sign ),lockroomGraphHandle_,true);
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
