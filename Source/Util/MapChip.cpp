#include "MapChip.h"

#include<fstream>
#include<json.hpp>
#include<ChipIndex.h>
#include<DxlibInclude.h>
#include<filesystem>
#include<random>

static std::vector<std::string> GetFilesName(const std::string& path)
{
	std::vector<std::string>ret;

	auto dir_it = std::filesystem::directory_iterator(path);

	for ( auto& p : dir_it )
	{
		ret.push_back(p.path().filename().stem().string());
	}

	return ret;
}

static int Random(int low,int high)
{
	static std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low,high);
	return dist(gen);
}

void MapChip::Initialize()
{
	roadChipHandle_ = LoadGraph("Resources/Chip/RoadChip.png");
	wallChipHandle_ = LoadGraph("Resources/Chip/WallChip.png");
}

void MapChip::MapLoad(const std::string& path)
{
	std::fstream file;
	file.open(path);
	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json jsonData;
	file >> jsonData;
	file.close();
	map_.resize(jsonData[ "1_Size" ][ 0 ][ 1 ]);
	for ( size_t i = 0; i < map_.size(); i++ )
	{
		map_[ i ].resize(jsonData[ "1_Size" ][ 0 ][ 0 ]);
	}


	size_t index = 0;
	for ( size_t i = 0; i < map_.size(); i++ )
	{
		for ( size_t j = 0; j < map_[ i ].size(); j++ )
		{
			map_[ i ][ j ] = jsonData[ "Map" ][ index ];
			index++;
		}
	}

	nlohmann::json& roomSettings = jsonData[ "3_RoomSettings" ];


	for ( size_t i = 0; i < jsonData[ "2_RoomNum" ][ 0 ]; i++ )
	{
		std::string directoryPath = "Resources/Export/Room/" + std::string(roomSettings[ i ][ "RoomName" ]);
		int32_t tmp = roomSettings[ i ][ "LeftTop" ][ 0 ];
		Vector2 leftTop;
		leftTop.x = float(tmp - 1);
		tmp = roomSettings[ i ][ "LeftTop" ][ 1 ];
		leftTop.y = float(tmp - 1);

		RoomInstallation(directoryPath,leftTop);
	}

}

const std::vector<std::vector<uint8_t>>& MapChip::GetMapChip() const
{
	return map_;
}

uint8_t MapChip::GetPosElement(int32_t x, int32_t y)const
{
	return map_[y / BLOCK_SIZE][x / BLOCK_SIZE];
}

uint8_t MapChip::GetNumOfArrayElement(int32_t x, int32_t y) const
{
	return map_[y][x];
}

void MapChip::Draw(const Vector2& screenPos)
{
	for ( size_t i = 0; i < map_.size(); i++ )
	{
		for ( size_t j = 0; j < map_[ i ].size(); j++ )
		{
			switch ( map_[ i ][ j ] )
			{
			case NONE:
				break;
			case ROAD:
				DrawRotaGraph3(( j * BLOCK_SIZE ) + screenPos.x,( i * BLOCK_SIZE ) + screenPos.y,0,0,0.5,0.5,0.0,roadChipHandle_,true);
				break;
			case DOOR:
				break;
			case ROOM:
				break;
			case LOCK_ROOM:
				break;
			case WALL:
				DrawRotaGraph3(( j * BLOCK_SIZE ) + screenPos.x,( i * BLOCK_SIZE ) + screenPos.y,0,0,0.5,0.5,0.0,wallChipHandle_,true);
				break;
			default:
				break;
			}
		}
	}
}

void MapChip::RoomInstallation(const std::string& directoryPath,const Vector2& leftTop)
{
	nlohmann::json jsonData;
	std::fstream file;
	std::string path;
	size_t index = 0;

	std::vector < std::string>files = GetFilesName(directoryPath);
	path = directoryPath + '/' + files[ Random(0,files.size() - 1) ] + ".json";

	file.open(path);
	if ( file.fail() )
	{
		assert(0);
	}


	file >> jsonData;
	file.close();

	for ( size_t i = 0; i < jsonData[ "1_Size" ][ 0 ][ 1 ]; i++ )
	{
		for ( size_t j = 0; j < jsonData[ "1_Size" ][ 0 ][ 0 ]; j++ )
		{
			map_[ leftTop.y + i ][ leftTop.x + j ] = jsonData[ "Room" ][ index ];
			index++;
		}
	}
}
