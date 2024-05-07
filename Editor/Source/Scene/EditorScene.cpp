#include "EditorScene.h"

#include"DxlibInclude.h"
#include"DxLib.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static ID3D11ShaderResourceView* GetImageResource11(int softimageHandle)
{

	ID3D11Device* device = static_cast< ID3D11Device* >( const_cast< void* >( GetUseDirect3D11Device() ) );
	ID3D11DeviceContext* context = static_cast< ID3D11DeviceContext* >( const_cast< void* >( GetUseDirect3D11DeviceContext() ) );

	ID3D11Texture2D* texture = ( ID3D11Texture2D* ) GetGraphID3D11Texture2D(softimageHandle);
	// ShaderResourceViewの作成
	ID3D11ShaderResourceView* pSRV = nullptr;
	device->CreateShaderResourceView(texture,nullptr,&pSRV);

	return pSRV;
}

void EditorScene::Initialize(int32_t screen)
{

	pSRV_ = GetImageResource11(screen);
	screen_ = screen;

	//変える予定
	mapCenter_.x = ( mapSize_.x * blockSize_ ) / 2;
	mapCenter_.y = ( mapSize_.y * blockSize_ ) / 2;

	editorMap.resize(mapSize_.y);
	for ( size_t i = 0; i < editorMap.size(); i++ )
	{
		editorMap[ i ].resize(mapSize_.x);
	}

	noneGraphHandle = LoadGraph("Resource/NoneChip");
	roadGraphHandle = LoadGraph("Resource/RoadChip");
	doorGraphHandle = LoadGraph("Resource/DoorChip");
	roomGraphHandle = LoadGraph("Resource/RoomChip");
	lockroomGraphHandle = LoadGraph("Resource/LockroomChip");
}

void EditorScene::Update()
{
	screenOldMousePos_ = screenMousePos_;

	int mouseWheelRotVol = GetMouseWheelRotVol();

	scale_.x += 0.1f * mouseWheelRotVol;
	scale_.y += 0.1f * mouseWheelRotVol;

	GetMousePoint(&screenMousePos_.x,&screenMousePos_.y);

	EditorMove();

	editorMousePos_ = GetEditorMousePos();

	if ( IsEditorMapWithin(editorMousePos_.x/blockSize_,editorMousePos_.y/blockSize_) )
	{

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

			ChipDraw(centerX,centerY,editorMap[i][j]);
			DrawBoxAA(( centerX - blockSizeHalf ),( centerY - blockSizeHalf ),( centerX + blockSizeHalf ),( centerY + blockSizeHalf ),0xffffffff,false);
		}
	}

	DrawBoxAA(( editorMousePos_.x / GetBlockSize() ) * 32,( editorMousePos_.y / GetBlockSize() ) * 32,( editorMousePos_.x / GetBlockSize() ) * 32 + 32,( editorMousePos_.y / GetBlockSize() ) * 32 + 32,0xffffffff,true);
}

void EditorScene::UIUpdate()
{
	ImGui::Begin("EditorView",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	EditorView();
	ImGui::End();


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
	ImGui::SetWindowSize({ ( float ) VIEW_WINDOW_SIZE.x,( float ) VIEW_WINDOW_SIZE.y });
	ImGui::SetWindowPos({ 0,0 });

	ImGui::Image(pSRV_,{ ( float ) VIEW_WINDOW_SIZE.x,( float ) VIEW_WINDOW_SIZE.y });
}

void EditorScene::EditorMove()
{
	if ( CheckHitKey(KEY_INPUT_LSHIFT) && GetMouseInput() & MOUSE_INPUT_LEFT )
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
	return x != -1 && y != -1 && x < mapSize_.x&& y < mapSize_.y;
}

void EditorScene::ChipDraw(size_t x,size_t y,int8_t chip)
{
	switch ( chip )
	{
	case NONE:
		DrawGraph(x+ blockSizeHalf_,y+ blockSizeHalf_,noneGraphHandle,true);
		break;
	case ROAD:
		DrawGraph(x + blockSizeHalf_,y + blockSizeHalf_,roadGraphHandle,true);
		break;
	case DOOR:
		DrawGraph(x + blockSizeHalf_,y + blockSizeHalf_,doorGraphHandle,true);
		break;
	case ROOM:
		DrawGraph(x + blockSizeHalf_,y + blockSizeHalf_,roomGraphHandle,true);
		break;
	case LOCK_ROOM:
		DrawGraph(x + blockSizeHalf_,y + blockSizeHalf_,lockroomGraphHandle,true);
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
