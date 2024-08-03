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
	roadChipHandle_ = LoadGraph(std::string("Resources/Chip/RoadChip.png"));
	wallChipHandle_ = LoadGraph(std::string("Resources/Chip/WallChip.png"));
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
			ChipIndex chip = jsonData[ "Map" ][ index ];

			switch ( chip )
			{
			case START:
				player_->SetPos({ 32.0f + 64.0f * j,32.0f + 64.0f * i });
				map_[ i ][ j ] = ChipIndex::NONE;
				break;
			case FLY_RANGE_ENEMY:
				map_[ i ][ j ] = ChipIndex::NONE;
				enemyManager_->SetEnemyPop(EnemyType::FLY,{ 32.0f + 64.0f * j,32.0f + 64.0f * i });
				break;
			case LONG_RANGE_ENEMY:
				map_[ i ][ j ] = ChipIndex::NONE;
				enemyManager_->SetEnemyPop(EnemyType::SHOOT,{ 32.0f + 64.0f * j,32.0f + 64.0f * i });
				break;
			case SHORT_RANGE_ENEMY:
				map_[ i ][ j ] = ChipIndex::NONE;
				enemyManager_->SetEnemyPop(EnemyType::ADJACENT,{ 32.0f + 64.0f * j,32.0f + 64.0f * i });
				break;
			case RANDOM_ENEMY:
				map_[ i ][ j ] = ChipIndex::NONE;
				enemyManager_->SetPosPop({ 32.0f + 64.0f * j,32.0f + 64.0f * i });
				break;

			case NONE:

			case ROAD:
			case WALL:

			case DOOR:
			case ROOM:
			case LOCK_ROOM:

			case NEXT:

			default:

				map_[ i ][ j ] = chip;
				break;
			}

			index++;
		}
	}
}

void MapChip::MapWrite(int32_t x,int32_t y,uint8_t index)
{
	map_[ y ][ x ] = index;
}

const std::vector<std::vector<uint8_t>>& MapChip::GetMapChip() const
{
	return map_;
}

std::vector<std::vector<uint8_t>>* MapChip::GetMapChipPtr()
{
	return &map_;
}

uint8_t MapChip::GetPosElement(int32_t x,int32_t y)const
{
	return map_[ y / BLOCK_SIZE ][ x / BLOCK_SIZE ];
}

Vector2 MapChip::GetPos(int32_t x,int32_t y) const
{
	return Vector2(x / BLOCK_SIZE,y / BLOCK_SIZE);
}

uint8_t MapChip::GetNumOfArrayElement(int32_t x,int32_t y) const
{
	int32_t posY = min(uint32_t(y),map_.size() - 1);
	posY = max(posY,0);

	int32_t posX = min(uint32_t(x),map_[ posY ].size() - 1);
	posX = max(posX,0);

	return map_[ posY ][ posX ];
}

const Vector2& MapChip::GetScreenPos() const
{
	return screenPos_;
}

const Vector2& MapChip::GetLeftTopPos() const
{
	return leftTopPos_;
}

const Vector2& MapChip::GetRightTopBottom() const
{
	return rightTopBottom_;
}

void MapChip::SetPlayer(Player* player)
{
	player_ = player;
}

void MapChip::SetEnemyManager(EnemyManager* enemyManager)
{
	enemyManager_ = enemyManager;
}

void MapChip::Draw(const Vector2& screenPos)
{
	screenPos_ = screenPos + Vector2(32,32);

	for ( size_t i = 0; i < map_.size(); i++ )
	{
		for ( size_t j = 0; j < map_[ i ].size(); j++ )
		{

			if ( i == 0 && j == 0 )
			{
				leftTopPos_ = { ( j * BLOCK_SIZE ) + screenPos.x - 32,( i * BLOCK_SIZE ) + screenPos.y - 32 };
			}

			if ( i == map_.size() - 1 && j == map_[ map_.size() - 1 ].size()-1 )
			{
				rightTopBottom_ = { ( j * BLOCK_SIZE ) + screenPos.x + 32,( i * BLOCK_SIZE ) + screenPos.y + 32 };
			}

			switch ( map_[ i ][ j ] )
			{
			case ROAD:
				DrawRotaGraph(( j * BLOCK_SIZE ) + screenPos.x,( i * BLOCK_SIZE ) + screenPos.y,1.0,0.0,roadChipHandle_,true);
				break;
			case WALL:
				DrawRotaGraph(( j * BLOCK_SIZE ) + screenPos.x,( i * BLOCK_SIZE ) + screenPos.y,1.0,0.0,wallChipHandle_,true);
				break;
			case NONE:
			case DOOR:
			case ROOM:
			case LOCK_ROOM:
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
