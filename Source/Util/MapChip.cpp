#include "MapChip.h"

#include<fstream>
#include<json.hpp>
#include<ChipIndex.h>
#include<DxlibInclude.h>

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
	map_.resize(jsonData[ "1_Size" ][ 0 ][1]);
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

}

const std::vector<std::vector<uint8_t>>& MapChip::GetMapChip() const
{
	return map_;
}

void MapChip::Draw(const Vector2& screenPos)
{
	for ( size_t i = 0; i < map_.size(); i++ )
	{
		for ( size_t j = 0; j < map_[i].size(); j++ )
		{
			switch ( map_[i][j] )
			{
			case NONE:
				break;
			case ROAD:
				DrawGraph(( j * BLOCK_SIZE ) + screenPos.x,( i * BLOCK_SIZE )+ screenPos.y,roadChipHandle_,true);
				break;
			case DOOR:
				break;
			case ROOM:
				break;
			case LOCK_ROOM:
				break;
			case WALL:
				DrawGraph(( j * BLOCK_SIZE ) + screenPos.x,(  i * BLOCK_SIZE ) + screenPos.y,wallChipHandle_,true);
				break;
			default:
				break;
			}
		}
	}
}
