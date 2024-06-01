#pragma once
#include<vector>
#include<string>

#include"Vector2.h"

class MapChip
{
private:

	const int32_t BLOCK_SIZE = 32;

	std::vector < std::vector<uint8_t>>map_;

	int32_t roadChipHandle_ = 0;
	int32_t wallChipHandle_ = 0;

public:

	void Initialize();

	void MapLoad(const std::string& path);

	const std::vector < std::vector<uint8_t>>& GetMapChip()const;

	void Draw(const Vector2& screenPos);
};
